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
