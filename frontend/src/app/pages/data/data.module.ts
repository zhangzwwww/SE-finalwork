import { NgModule } from '@angular/core';

import { DataComponent } from './data.component';
import { DataRoutingModule } from './data-routing.module';

@NgModule({
  imports: [DataRoutingModule],
  declarations: [DataComponent],
  exports: [DataComponent]
})

export class DataModule {
}
