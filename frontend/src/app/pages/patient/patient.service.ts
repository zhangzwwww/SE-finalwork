import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';

@Injectable()
export class PatientService {
  constructor(private http: HttpClient) {
  }

  auth(): Observable<any> {
    return this.http.post('api/v1/auth/token', {
      email: 'test@test.com',
      password: 'hello123'
    }, { observe: 'response' });
  }

  private getToken(): any {
    return localStorage.getItem('X-Auth-Token');
  }

  getPatient(token: string): Observable<any> {
    return this.http.get('api-patient/v1/patient', { headers: { 'X-Auth-Token': token }, observe: 'response' });
  }

  deletePatient(patient: string): Observable<any> {
    const token: any = this.getToken();
    return this.http.delete<void>('api-patient/v1/patient/' + patient, { headers: { 'X-Auth-Token': token } });
  }

  createPatient(patient: any): Observable<any> {
    const token: any = this.getToken();
    return this.http.post<void>('api-patient/v1/patient', patient, { headers: { 'X-Auth-Token': token }, observe: 'response' });
  }

  deleteImage(id: string): Observable<any> {
    const token: any = this.getToken();
    return this.http.delete('api-patient/v1/image', {
      params: { id: id },
      headers: { 'X-Auth-Token': token },
      observe: 'response'
    });
  }

  getCTime(patientId: string): Observable<any> {
    const token: any = this.getToken();
    return this.http.get('api-patient/v1/image/ctime', {
      params: { patientId: patientId },
      headers: { 'X-Auth-Token': token },
      observe: 'response'
    });
  }

  getImages(patientId: string, ctime: string): Observable<any> {
    const token: any = this.getToken();
    return this.http.get('api-patient/v1/image', {
      params: { patientId: patientId, ctime: ctime },
      headers: { 'X-Auth-Token': token },
      observe: 'response'
    });
  }

  uploadImage(form: FormData): Observable<any>{
    const token: any = this.getToken();
    return this.http.post('api-patient/v1/image', form, {
      headers: {'X-Auth-Token': token},
      observe: 'response',
    });
  }
}
