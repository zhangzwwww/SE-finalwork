import { Component, OnInit } from '@angular/core';
import { PatientService } from './patient.service';
import { FormBuilder, FormGroup } from '@angular/forms';
import { DatePipe } from '@angular/common';

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
  createPatientIsVisible = false;
  validateForm!: FormGroup;

  constructor(private patientService: PatientService, private fb: FormBuilder, private datePipe: DatePipe) {
  }

  ngOnInit(): void {
    this.createPatientIsVisible = false;
    this.token = localStorage.getItem('X-Auth-Token');
    console.log(this.token);
    this.getAllPatient();
    this.validateForm = this.fb.group({
      name: [null],
      gender: [null],
      birth: [null],
      age: [null],
    });
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

  startCreatePatient(): void {
    this.createPatientIsVisible = true;
  }

  handleCreatePatientCancel(): void {
    this.createPatientIsVisible = false;
  }

  createPatient(): void {
    const value = this.validateForm.value;
    value.birth = this.datePipe.transform(this.validateForm.value.birth, 'yyyy-MM-dd');
    this.patientService.createPatient(value).subscribe(() => {
      this.createPatientIsVisible = false;
      this.validateForm.reset();
      this.getAllPatient();
    });
  }
}
