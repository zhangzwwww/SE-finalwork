import { Component, OnInit } from '@angular/core';
import { WelcomeService } from './welcome.service';

@Component({
  selector: 'app-welcome',
  templateUrl: './welcome.component.html',
  styleUrls: ['./welcome.component.css']
})
export class WelcomeComponent implements OnInit {

  users = [];

  constructor(private welcomeService: WelcomeService) {
  }

  ngOnInit(): void {
    this.welcomeService.getAllUser().subscribe((response) => {
      this.users = response.body;
      console.log(this.users);
    });
  }

  getAllUsers(): void {
    this.welcomeService.getAllUser().subscribe((response) => {
      this.users = response.body;
    });
  }

  deleteUser(email): void {
    this.welcomeService.deleteUser(email).subscribe(() => {
      this.getAllUsers();
    });
  }
}
