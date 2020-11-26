from flask import request, Response
from flask_restful import Resource
from model import Patient, PatientService
import uuid
from datetime import datetime
import json
from app import redis


def register_routes(api, app):
    api.add_resource(PatientController,
                     '/v1/patient')
    api.add_resource(PatientWithIdController, '/v1/patient/<patientId>')


class PatientController(Resource):
    def get(self):
        token = request.headers.get('X-Auth-Token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        patients = PatientService.getAllPatient()
        patientList = []
        for patient in patients:
            patientList.append(patient.serialize())
        resp = Response(json.dumps(patientList),
                        status=200, mimetype='applications/json')
        return resp

    def post(self):
        token = request.headers.get('X-Auth-Token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        name = request.json.get('name')
        gender = request.json.get('gender')
        birth = request.json.get('birth')
        age = request.json.get('age')
        id = uuid.uuid5(uuid.NAMESPACE_DNS, str(datetime.now()))

        PatientService.createPatient(Patient(id=id, name=name, gender=gender,
                                             birth=birth, age=age))

        resp = Response(status=201, mimetype='application/json')
        return resp


class PatientWithIdController(Resource):
    def get(self, patientId):
        token = request.headers.get('X-Auth-Token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        patient = PatientService.getPatientById(patientId)
        resp = Response(json.dumps(patient.serialize()),
                        status=200, mimetype='application/json')
        return resp

    def put(self, patientId):
        token = request.headers.get('X-Auth-Token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        patient = Patient(id=request.json.get('id'),
                          name=request.json.get('name'),
                          gender=request.json.get('gender'),
                          birth=request.json.get('birth'),
                          age=request.json.get('age'))

        PatientService.ModifyPatient(patientId, patient)
        return "modify succeed", 204

    def delete(self, patientId):
        token = request.headers.get('X-Auth-Token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        PatientService.DeletePatient(patientId)
        return "delete successfully", 204


def existsToken(token):
    return redis.containsKey(token)
