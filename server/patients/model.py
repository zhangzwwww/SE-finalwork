from app import db


class Patient(db.Model):
    __tablename__ = 'patient'

    id = db.Column(db.String, unique=True, primary_key=True)
    name = db.Column(db.String)
    gender = db.Column(db.String)
    birth = db.Column(db.DateTime)
    age = db.Column(db.Integer)

    def serialize(self):
        return {
            'id': self.id,
            'name': self.name,
            'gender': self.gender,
            'birth': str(self.birth),
            'age': self.age,
        }


class Mark(db.Model):
    __tablename__ = 'mark'

    id = db.Column(db.String, unique=True, primary_key=True)
    imageId = db.Column(db.String)
    layer = db.Column(db.Float)
    view = db.Column(db.Float)
    topX = db.Column(db.Float)
    topY = db.Column(db.Float)
    bottomX = db.Column(db.Float)
    bottomY = db.Column(db.Float)

    def serialize(self):
        return {
            'id': self.id,
            'imageId': self.imageId,
            'layer': self.layer,
            'view': self.view,
            'topX': self.topX,
            'topY': self.topY,
            'bottomX': self.bottomX,
            'bottomY': self.bottomY,
        }


class PatientService:
    def createPatient(patient):
        db.session.add(patient)
        db.session.commit()
        return True

    def getAllPatient():
        patients = Patient.query.all()
        return patients

    def ModifyPatient(patientId, patient):
        p = Patient.query.filter_by(id=patientId).first()
        p.name = patient.name
        p.gender = patient.gender
        p.birth = patient.birth
        p.age = patient.age
        db.session.commit()
        return True

    def DeletePatient(patientId):
        patient = Patient.query.filter_by(id=patientId).first()
        db.session.delete(patient)
        db.session.commit()
        return True

    def getPatientById(patientId):
        patient = Patient.query.filter_by(id=patientId).first()
        return patient


class MarkService:
    def createMark(mark):
        db.session.add(mark)
        db.session.commit()
        return True

    def getMarkByImageId(imageId):
        marks = Mark.query.filter_by(imageId=imageId)
        return marks
