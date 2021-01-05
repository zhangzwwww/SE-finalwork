import { RouterModule, Routes } from '@angular/router';
import { UserComponent } from './user.component';
import { NgModule } from '@angular/core';

const routes: Routes = [
  { path: '', component: UserComponent },
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule]
})
export class UserRoutingModule {
}
