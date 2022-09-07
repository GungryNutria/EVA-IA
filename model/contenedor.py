class Contenedor:

    def __init__(self,container,score):
        self._container = container
        self._score = score
    def setContainer(self,container):
        self._container = container
    def getContainer(self):
        return self.container
    def setScore(self, score):
        self._score = score
    def getScore(self):
        return self.score
    def __str__(self):
        return f'Container: {self._container}, Score: {self._score}'