from flask import request, Response
from flask_restful import Resource
import flask
from io import StringIO
from PIL import Image

from app import redis, model

import torchvision.transforms as transforms
import torch

classes = ['glioma_tumor', 'meningioma_tumor', 'no_tumor', 'pituitary_tumor']


def register_routes(api, app):
    api.add_resource(PredictController, '/v1/predict')


class PredictController(Resource):
    def post(self):
        print("Start predicting...")
        token = request.headers.get('X-Auth-Token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        f = flask.request.files['tumor_image']

        img = Image.open(f)
        transform = transforms.Compose([
            transforms.RandomResizedCrop(224),
            transforms.ToTensor(),
            transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
        ])
        img = transform(img)
        img = img.unsqueeze(0)

        with torch.no_grad():
            model.eval()
            py = model(img)

        res = torch.max(py, 1)[1].data.numpy()[0]
        return classes[res], '200'


def existsToken(token):
    return redis.containsKey(token)
