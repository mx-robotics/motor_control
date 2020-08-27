### Motor Cable Orderings

There are 6 different ways to arrange motor cables (usually depicted as phases or terminals) since it is done via non-repetitive permutations i.e. simple factorial (3! = 6).

For a sequence to work, there is one basic requirement: The phases must be 120° shifted from each other. Our software already generates 3 120° phase shifted signals, lets call them U,V,W. The question is how to arrange them. The options are:

1. | U | V   | W   |
|---|-----|-----|
| 0 | 120 | 240 |

Satisfies the requirement.

2. 
| W | V   | U   |
|---|-----|-----|
| 120 | 0 | 240 |

	U and W swapped. Doesnt satisfiy the requirement: Phase shift between W and  U is 240°

3.    | V| U   | W   |
	 |---|-----|-----|
     |  120 | 0 | 240 |
         
      U and V swapped. Doesnt satisfiy the requirement: Phase shift between W and  U is 240°

4. | W | U   | V   |
|---|-----|-----|
| 240 | 0 | 120 |

	W and V swapped from the 3. configuration. This is actually just a shift to the right from the 1. configuration. Satisfies the requirement.

5. | U | W   | V   |
|---|-----|-----|
| 0 | 240 | 120 |

      W and V swapped. Doesnt satisfiy the requirement:: Phase shift between V and  U is 240°


6. 
| V | W  | U   |
|---|-----|-----|
| 120 | 240 | 0 |

	U and V swapped from the 5. configuration. This is actually just a shift to the left from the 1. configuration. Satisfies the requirement.
	
	
The question is: Does all the configurations that satisfy the requirement of 120° phase shift work with the same software configuration? The rotor flux angle offset might be different for each config.

It is also clear that if the motor terminals are plugged in the wrong direction (ABC vs CBA) the motor doesnt spin.
	
	
	

