import { NgModule } from '@angular/core';

import { WelcomeRoutingModule } from './welcome-routing.module';

import { WelcomeComponent } from './welcome.component';
import { CommonModule } from '@angular/common';
import { PatientRoutingModule } from '../patient/patient-routing.module';
import { NzTableModule } from 'ng-zorro-antd/table';
import { NzDividerModule } from 'ng-zorro-antd/divider';
import { NzButtonModule } from 'ng-zorro-antd/button';
import { NzIconModule } from 'ng-zorro-antd/icon';
import { NzModalModule } from 'ng-zorro-antd/modal';
import { NzFormModule } from 'ng-zorro-antd/form';
import { NzInputModule } from 'ng-zorro-antd/input';
import { NzRadioModule } from 'ng-zorro-antd/radio';
import { NzDatePickerModule } from 'ng-zorro-antd/date-picker';
import { NzSelectModule } from 'ng-zorro-antd/select';
import { NzUploadModule } from 'ng-zorro-antd/upload';
import { FormsModule, ReactiveFormsModule } from '@angular/forms';
import { WelcomeService } from './welcome.service';
import { NzPopconfirmModule } from 'ng-zorro-antd/popconfirm';


@NgModule({
  imports: [
    WelcomeRoutingModule,
    CommonModule,
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
    NzUploadModule,
    NzPopconfirmModule,
    ReactiveFormsModule,
    FormsModule,
  ],
  declarations: [WelcomeComponent],
  exports: [WelcomeComponent],
  providers: [WelcomeService]
})
export class WelcomeModule {
}
