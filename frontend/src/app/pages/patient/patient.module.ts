import { NgModule } from '@angular/core';
import { PatientRoutingModule } from './patient-routing.module';
import { PatientComponent } from './patient.component';
import { PatientService } from './patient.service';
import { CommonModule } from '@angular/common';
import { NzTableModule } from 'ng-zorro-antd/table';
import { NzDividerModule } from 'ng-zorro-antd/divider';

@NgModule({
  imports: [
    CommonModule,
    PatientRoutingModule,
    NzTableModule,
    NzDividerModule,
  ],
  declarations: [PatientComponent],
  exports: [PatientComponent],
  providers: [PatientService]
})
export class PatientModule {
}
