import { RouterModule, Routes } from '@angular/router';
import { DataComponent } from './data.component';
import { NgModule } from '@angular/core';

const routes: Routes = [
  { path: '', component: DataComponent },
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule]
})

export class DataRoutingModule {
}
