import { Component, OnInit } from '@angular/core';
import { PatientService } from './patient.service';
import { FormBuilder, FormGroup } from '@angular/forms';
import { DatePipe } from '@angular/common';
import { NzUploadFile } from 'ng-zorro-antd/upload';

@Component({
  selector: 'app-patient',
  templateUrl: './patient.component.html',
  styleUrls: ['./patient.component.css']
})
export class PatientComponent implements OnInit {
  patients: any;
  token: any;
  images: any;
  cTimes = [];
  cTime: any;
  createPatientIsVisible = false;
  validateForm!: FormGroup;
  imageIsVisible = false;
  chosenPatientId: string;

  fileUploadModalVisible = false;
  file: any;

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

  deleteImage(id: string): void {
    this.patientService.deleteImage(id).subscribe(() => {
      this.getCTime(this.chosenPatientId);
    });
  }

  getImages(patientId: string, cTime: string): void {
    this.patientService.getImages(patientId, cTime).subscribe((response) => {
      this.images = response.body;
    });
  }

  showImageModelCancel(): void {
    this.cTimes = [];
    this.cTime = null;
    this.images = null;
    this.imageIsVisible = false;
  }

  showImageModal(patientId: string): void {
    this.chosenPatientId = patientId;
    this.patientService.getCTime(patientId).subscribe((response) => {
      this.cTimes = response.body;
      console.log(this.cTimes);
      this.imageIsVisible = true;
    });
  }

  startCreatePatient(): void {
    this.createPatientIsVisible = true;
  }

  handleCreatePatientCancel(): void {
    this.validateForm.reset();
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

  showFileUploadModal(): void {
    this.fileUploadModalVisible = true;
  }

  cancelFileUploadModal(): void {
    this.file = null;
    this.fileUploadModalVisible = false;
  }

  fileUpload(): void {
    const formData = new FormData();
    formData.append('uploaded_file', this.file[0]);
    formData.append('patientId', this.chosenPatientId);
    formData.append('filename', this.file[0].name);
    this.patientService.uploadImage(formData).subscribe((response) => {
      this.file = null;
      this.getImages(this.chosenPatientId, this.cTime);
      this.fileUploadModalVisible = false;
    });
  }

  beforeUpload = (file: NzUploadFile): boolean => {
    this.file = [file];
    return false;
  }
}
