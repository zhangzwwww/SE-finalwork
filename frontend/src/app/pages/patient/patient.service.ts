import { HttpClient, HttpParams } from '@angular/common/http';
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

  getCTime(patientId: string): Observable<any> {
    const token: any = this.getToken();
    const p = new HttpParams();
    p.append('patientId', patientId);
    return this.http.get('api-patient/v1/image/ctime/', { params: p, headers: { 'X-Auth-Token': token }, observe: 'response' });
  }

  getImages(patientId: string, ctime: string): Observable<any> {
    const token: any = this.getToken();
    const p = new HttpParams();
    p.append('patientId', patientId);
    p.append('ctime', ctime);
    return this.http.get('api-patient/v1/images', {params: p, headers: {'X-Auth-Token': token}, observe: 'response'});
  }
}
