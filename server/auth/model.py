from app import db


class User(db.Model):
    __tablename__ = 'user'

    id = db.Column(db.String, unique=True, primary_key=True)
    email = db.Column(db.String, unique=True)
    ctime = db.Column(db.DateTime)
    role = db.Column(db.String)
    password = db.Column(db.String)

    def serialize(self):
        return {
            'id': self.id,
            'email': self.email,
            'ctime': str(self.ctime),
            'role': self.role,
        }


class UserService:

    def createUser(user):
        db.session.add(user)
        db.session.commit()
        return True

    def getUser(email):
        user = User.query.filter_by(email=email).first()
        return user

    def getUserByEmailAndPassword(email, password):
        user = User.query.filter_by(email=email, password=password).first()
        return user

    def deleteUserByEmail(email):
        user = User.query.filter_by(email=email).first()
        print(user.id)
        db.session.delete(user)
        db.session.commit()
        return True
