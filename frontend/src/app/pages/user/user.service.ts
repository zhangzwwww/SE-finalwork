import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable()
export class UserService {
  constructor(private http: HttpClient) {
  }

  auth(): Observable<any> {
    return this.http.post('api/v1/auth/token', {
      email: 'test@test.com',
      password: 'hello123'
    }, { observe: 'response' });
  }

  getAllUser(): Observable<any> {
    const token: any = localStorage.getItem('X-Auth-Token');
    return this.http.get('api/v1/token', { headers: { 'X-Auth-Token': token }, observe: 'response' });
  }
}
