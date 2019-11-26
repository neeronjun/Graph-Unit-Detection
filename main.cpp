//
//  main.cpp
//  Helloworld
//
//  Created by Niranjan Rai on 11/24/19.
//  Copyright © 2019 nrai. All rights reserved.
//

#include <iostream>
#include<math.h>

using namespace std;



int main() {
    // insert code here...
    float c_intUB, c_need = 0.0, c_extra, c_obtained, c_in = 0.0, c_out = 0.0;
    float nb, D2 = 0.0, r = 0.1;
    int N = 30000;
    
    nb = powf(0.5, D2/2) * (N-1) * powf(2, D2) * powf(r, D2);
    c_intUB = c_out - c_in;
    c_obtained = c_in + c_intUB;
    c_extra = c_obtained - c_need;
    
    cout << "Hello, World!\n";
    return 0;
}
