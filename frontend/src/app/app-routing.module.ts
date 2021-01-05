import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

const routes: Routes = [
  { path: '', pathMatch: 'full', redirectTo: '/welcome' },
  { path: 'welcome', loadChildren: () => import('./pages/welcome/welcome.module').then(m => m.WelcomeModule) },
  { path: 'patient', loadChildren: () => import('./pages/patient/patient.module').then(m => m.PatientModule) },
  { path: 'user', loadChildren: () => import('./pages/user/user.module').then(m => m.UserModule)},
  { path: 'data', loadChildren: () => import('./pages/data/data.module').then(m => m.DataModule)},
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule {
}
