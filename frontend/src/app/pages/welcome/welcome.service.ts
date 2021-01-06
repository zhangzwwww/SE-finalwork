import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable()
export class WelcomeService {
  constructor(private http: HttpClient) {
  }

  private getToken(): any {
    return localStorage.getItem('X-Auth-Token');
  }

  getAllUser(): Observable<any> {
    const token: any = this.getToken();
    return this.http.get('api/v1/user', { headers: { 'X-Auth-Token': token }, observe: 'response' });
  }

  deleteUser(email: string): Observable<any> {
    const token: any = this.getToken();
    return this.http.delete('api/v1/auth/deleteUser', {
      params: { email: email },
      headers: { 'X-Auth-Token': token },
      observe: 'response'
    });
  }
}
