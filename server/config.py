import yaml
import os


class Config():
    def __init__(self, configFileName='config.yaml'):
        filePath = os.path.dirname(__file__)
        yamlPath = os.path.join(filePath, configFileName)
        f = open(yamlPath, 'r', encoding='utf-8')
        self.configs = yaml.load(f.read(), Loader=yaml.Loader)

    def getConsulHost(self):
        return self.configs['consul']['host']

    def getConsulPort(self):
        return self.configs['consul']['port']
