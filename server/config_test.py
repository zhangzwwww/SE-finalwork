import unittest
import os
import yaml
import warnings
from config import Config


class TestConfig(unittest.TestCase):

    @classmethod
    def setUpClass(self):
        warnings.simplefilter('ignore', ResourceWarning)
        fakeConfigObj = {
            'consul': {
                'host': '1.1.1.1',
                'port': '80'
            }
        }
        filePath = os.path.dirname(__file__)
        self.yamlPath = os.path.join(filePath, 'fakeConfig.yaml')
        if not os.path.exists(self.yamlPath):
            self.f = open(self.yamlPath, 'w')
            yaml.dump(fakeConfigObj, self.f)
            self.f.close()
        self.config = Config('fakeConfig.yaml')

    def test_init(self):
        self.assertIsNotNone(self.config.configs)

    def test_getConsulHost(self):
        self.assertEqual(self.config.getConsulHost(), "1.1.1.1")

    def test_getConsulPort(self):
        self.assertEqual(self.config.getConsulPort(), "80")

    @classmethod
    def tearDownClass(self):
        if os.path.exists(self.yamlPath):
            os.remove(self.yamlPath)
