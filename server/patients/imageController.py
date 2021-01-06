from flask import request, Response
from flask_restful import Resource
import flask
import datetime
import bson.binary
import bson.objectid
import bson.errors
import pymongo
import json
import uuid

from model import Mark, MarkService

from app import mongodb
from app import redis


def register_image_routes(api, app):
    api.add_resource(ImageController, '/v1/image')
    api.add_resource(ImageDateController, '/v1/image/ctime')
    api.add_resource(MarkController, '/v1/image/mark')


class MarkController(Resource):
    def get(self):
        token = request.headers.get('X-Auth-Token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        marks = MarkService.getMarkByImageId(request.args['imageId'])
        markList = []
        for mark in marks:
            markList.append(mark.serialize())
        resp = Response(json.dumps(markList),
                        status=200, mimetype='applications/json')
        return resp

    def post(self):
        token = request.headers.get('X-Auth-Token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        topX = request.json.get('topX')
        topY = request.json.get('topY')
        bottomX = request.json.get('bottomX')
        bottomY = request.json.get('bottomY')
        layer = request.json.get('layer')
        view = request.json.get('view')
        imageId = request.json.get('imageId')
        id = uuid.uuid5(uuid.NAMESPACE_DNS, str(datetime.datetime.now()))

        MarkService.createMark(Mark(id=id, imageId=imageId, topX=topX,
                                    topY=topY, bottomX=bottomX,
                                    bottomY=bottomY, layer=layer,
                                    view=view))
        resp = Response(status=201, mimetype='application/json')
        return resp


class ImageController(Resource):
    def get(self):
        token = request.headers.get('x-auth-token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        args = request.args

        patientId = args['patientId']
        time = args['ctime']

        contents = mongodb.files.find({'patientId': patientId, 'ctime': time})
        data = [{'id': str(d['id']),
                 'name': d['name'],
                 'content': str(d['content'], encoding='ISO-8859-1'),
                 'ctime': d['ctime'],
                 'patientId': d['patientId']} for d in contents]
        resp = Response(json.dumps(data, ensure_ascii=False),
                        status=200, mimetype='applications/json')

        return resp

    def post(self):
        token = request.headers.get('X-Auth-Token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        patientId = request.form['patientId']
        filename = request.form['filename']

        f = flask.request.files['uploaded_file']
        saveFile(f, filename, patientId)
        return 'succeed', 200

    def delete(self):
        token = request.headers.get('X-Auth-Token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        args = request.args
        id = args['id']

        mongodb.files.delete_one({"id": id})

        return 200


class ImageDateController(Resource):
    def get(self):
        token = request.headers.get('X-Auth-Token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        args = request.args

        patientId = args['patientId']
        contents = mongodb.files.find({'patientId': patientId})
        d = [d['ctime'] for d in contents]
        data = [{'ctime': d} for d in list(set(d))]
        resp = Response(json.dumps(data),
                        status=200, mimetype='applications/json')
        return resp


def saveFile(content, name, patientId):
    c = dict(
        id=uuid.uuid5(uuid.NAMESPACE_DNS, str(datetime.datetime.now())),
        content=bson.binary.Binary(content.read()),
        ctime=str(datetime.date.today()),
        name=name,
        patientId=patientId,
    )

    try:
        mongodb.files.save(c)
    except pymongo.errors.DuplicateKeyError:
        pass


def existsToken(token):
    return redis.containsKey(token)
