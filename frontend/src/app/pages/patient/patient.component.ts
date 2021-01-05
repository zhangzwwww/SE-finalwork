import { Component, OnInit } from '@angular/core';
import { PatientService } from './patient.service';

@Component({
  selector: 'app-patient',
  templateUrl: './patient.component.html',
  styleUrls: ['./patient.component.css']
})
export class PatientComponent implements OnInit {
  patients: any;
  token: any;
  images: any;
  cTimes: any;
  cTime: any;

  constructor(private patientService: PatientService) {
  }

  ngOnInit(): void {
    this.token = localStorage.getItem('X-Auth-Token');
    console.log(this.token);
    this.getAllPatient();
  }

  getGender(gender: string): string {
    if (gender === '0') {
      return '男';
    } else if (gender === '1') {
      return '女';
    } else {
      return 'unknown';
    }
  }

  getAllPatient(): void {
    this.patientService.getPatient(this.token).subscribe((response) => {
      console.log(response.body);
      this.patients = response.body;
    });
  }

  deletePatient(patientId: string): void {
    this.patientService.deletePatient(patientId).subscribe(() => {
      this.getAllPatient();
    });
  }

  getCTime(patientId: string): void {
    this.patientService.getCTime(patientId).subscribe((response) => {
      this.cTimes = response.body;
    });
  }

  getImages(patientId: string, cTime: string): void {
    this.patientService.getImages(patientId, cTime).subscribe((response) => {
      this.images = response.body;
    });
  }
}
