import { NgModule } from '@angular/core';
import { PatientRoutingModule } from './patient-routing.module';
import { PatientComponent } from './patient.component';
import { PatientService } from './patient.service';
import { CommonModule, DatePipe } from '@angular/common';
import { NzTableModule } from 'ng-zorro-antd/table';
import { NzDividerModule } from 'ng-zorro-antd/divider';
import { NzIconModule } from 'ng-zorro-antd/icon';
import { NzButtonModule } from 'ng-zorro-antd/button';
import { NzModalModule } from 'ng-zorro-antd/modal';
import { NzFormModule } from 'ng-zorro-antd/form';
import { NzInputModule } from 'ng-zorro-antd/input';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { NzRadioModule } from 'ng-zorro-antd/radio';
import { NzDatePickerModule } from 'ng-zorro-antd/date-picker';
import { NzSelectModule } from 'ng-zorro-antd/select';
import { NzUploadModule } from 'ng-zorro-antd/upload';
import { NzPopconfirmModule } from 'ng-zorro-antd/popconfirm';

@NgModule({
  imports: [
    CommonModule,
    PatientRoutingModule,
    NzTableModule,
    NzDividerModule,
    NzButtonModule,
    NzIconModule,
    NzModalModule,
    NzFormModule,
    NzInputModule,
    NzRadioModule,
    NzDatePickerModule,
    NzSelectModule,
    NzPopconfirmModule,
    NzUploadModule,
    ReactiveFormsModule,
    FormsModule,
  ],
  declarations: [PatientComponent],
  exports: [PatientComponent],
  providers: [PatientService, DatePipe]
})
export class PatientModule {
}
