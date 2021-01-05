import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { url } from './global';
import { Injectable } from '@angular/core';

@Injectable()
export class AppService {
  constructor(private http: HttpClient) {
  }

  auth(): Observable<any> {
    return this.http.post('api/v1/auth/token', {
      email: 'test@test.com',
      password: 'hello123'
    }, {observe: 'response'});
  }
}
