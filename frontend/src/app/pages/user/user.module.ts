import { UserRoutingModule } from './user-routing.module';
import { UserComponent } from './user.component';
import { NgModule } from '@angular/core';
import { UserService } from './user.service';
import { CommonModule } from '@angular/common';
import { NzTableModule } from 'ng-zorro-antd/table';

@NgModule({
  imports: [
    CommonModule,
    UserRoutingModule,
    NzTableModule,
  ],
  declarations: [UserComponent],
  exports: [UserComponent],
  providers: [UserService]
})
export class UserModule {
}
