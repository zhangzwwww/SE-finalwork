from redis import StrictRedis


class Redis():
    def __init__(self, host, port):
        self.redis = StrictRedis(host=host, port=port, db=0, password=None)

    def addKey(self, key, value):
        self.redis.set(key, value)

    def setExpireTime(self, key, expireTime) -> bool:
        return self.redis.expire(key, expireTime)

    def deleteKey(self, key):
        self.redis.delete(key)

    def containsKey(self, key):
        return self.redis.exists(key)

    def getValue(self, key):
        return self.redis.get(key)
