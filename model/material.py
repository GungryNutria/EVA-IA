class Material:
    
    def __init__(self,material, score):
        self.material = material
        self.score = score
    
    def getMaterial(self):
        return self.material
    
    def getScore(self):
        return self.score
    
    def setScore(self,score):
        self.score = score
    
    def setMaterial(self,material):
        self.material = material