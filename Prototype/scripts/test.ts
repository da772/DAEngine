import { Employee } from "./lib";

import { Component } from "./core/component";

export class TestComponent implements Component {

    e : Employee = new Employee(1, "Steve");

    initialize(): void {
        print("script init")
    }
    update(dt: number): void {
        let  sum = 0;
        for (let i = 0; i < 1e2; i++) {
            let ff = Math.sqrt(i);
            sum += ff;
       }

    }
        
    shutdown(): void {
        print("shut down");
    }
    
}

export let component : TestComponent = new TestComponent();
