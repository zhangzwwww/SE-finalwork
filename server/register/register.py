import consul


class Consul(object):
    """Consul Class

    Used to register service to consul

    for example you want to register a service 'test'

    before your register your service you need to config consul

    host = "0.0.0.0" # enter the host of your consul
    port = "0000"    # enter the port of your consul
    consul_client = Consul(host, port)

    then you can register your service

    host = "0.0.0.0" # service host
    port = 0000      # service port NEED TO BE INTEGER
    name = "test"    # service name
    consul_client.RegisterService(name, host, port)

    """
    def __init__(self, host, port):
        self._consul = consul.Consul(host, port)

    def RegisterService(self, name, host, port, tags=[],
                        interval="5s", timeout="30s", deregister="30s"):
        self._consul.agent.service.register(
            name,
            name,
            host,
            port,
            tags,
            check=consul.Check().tcp(host, port, interval, timeout, deregister)
        )

    def DeregisterService(self, name):
        self._consul.agent.service.deregister(name)

    def GetService(self, name):
        """
        Return service and address of the service
        """
        services = self._consul.agent.services()
        service = services.get(name)
        if not service:
            return None, None
        addr = "{0}:{1}".format(service['Address'], service['Port'])
        return service, addr
