from flask import request, Response
from flask_restful import Resource
from model import User, UserService
import uuid
from datetime import datetime
import hashlib
import json
from app import redis


def register_routes(api, app):
    api.add_resource(AuthToken, '/v1/auth/token')
    api.add_resource(AuthUser, '/v1/auth/register')


    # need to be deleted only for test
    api.add_resource(DeleteUser, '/v1/auth/deleteUser')


class AuthToken(Resource):
    def get(self):
        token = request.headers.get('X-Auth-Token')
        if existsToken(token):
            email = getValue(token)
            user = UserService.getUser(email)

            resp = Response(json.dumps(user.serialize()),
                            status=200, mimetype='applications/json')
            resp.headers['X-Auth-Token'] = token
            return resp
        else:
            return "token unavailable", 401

    def post(self):
        email = request.json.get('email')
        password = hashlib.sha224(
            request.json.get('password').encode('utf-8')).hexdigest()

        user = UserService.getUserByEmailAndPassword(email, password)

        if user is None:
            return "email or password incorrect", 401

        token = uuid.uuid4().hex
        addToken(token, email)

        resp = Response(status=200, mimetype='application/json')
        resp.headers['X-Auth-Token'] = token
        return resp

    def head(self):
        token = request.headers.get('X-Auth-Token')
        if existsToken(token):
            return "ok", 200
        return "Invalid token", 401

    def delete(self):
        token = request.headers['X-Auth-Token']
        revokeToken(token)
        return "deleted successfully", 204

# only for test need to be deleted
class DeleteUser(Resource):
    def delete(self):
        args = request.args
        email = args['email']
        UserService.deleteUserByEmail(email)
        return "deleted successfully", 204


class AuthUser(Resource):
    def post(self):
        email = request.json.get('email')
        user = UserService.getUser(email)
        if user is None:
            id = uuid.uuid5(uuid.NAMESPACE_DNS, email)
            role = 'user'
            ctime = datetime.now()
            pwd = hashlib.sha224(
                request.json.get('password').encode('utf-8')).hexdigest()
            UserService.createUser(User(id=id, ctime=ctime, role=role,
                                        email=email, password=pwd))

            token = uuid.uuid4().hex

            resp = Response(status=201, mimetype='application/json')
            resp.headers['X-Auth-Token'] = token

            addToken(token, email)

            return resp
        else:
            print("user already registered!")

            resp = Response(status=409)

            return resp


def addToken(token, value, expireTime=30000):
    redis.addKey(token, value)
    redis.setExpireTime(token, expireTime)


def revokeToken(token):
    redis.deleteKey(token)


def existsToken(token):
    return redis.containsKey(token)


def getValue(token):
    return redis.getValue(token)
