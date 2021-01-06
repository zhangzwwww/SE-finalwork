import { Component, OnInit } from '@angular/core';
import { AppService } from './app.service';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit {
  constructor(private appService: AppService) {
  }

  ngOnInit(): void {
    this.appService.auth().subscribe((response) => {
      localStorage.setItem('X-Auth-Token', response.headers.get('X-Auth-Token'));
    });
  }

  jumpToClient(): void {
    window.location.href = 'https://github.com/zhangzwwww/SE-finalwork/releases';
  }
}
