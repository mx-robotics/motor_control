* https://stackoverflow.com/questions/3992980/c-inline-member-function-in-cpp-file

* https://stackoverflow.com/questions/18860895/how-to-initialize-static-members-in-the-header



*  There is only one ftm0_isr function that is being triggered by either
     - FTM0 TOF(Timer Overflow Event) currently in use
    - Channel (n) Interrupt: FTM0_CnSC [Channel Flag] This interrupt comes when the CnV value is matched. 
     [CHnIE] = 1 must be set, The interrupt sets the relevant bit(CHF) at FTMO_C6SC. To clear: FTMO_C6SC &= ~FTM_CSC_CHF
 
      ``#define FTM_CSC_CHF			0x80 ``				// Channel Flag
      
      `` #define FTM_CSC_CHIE			0x40	``	// Channel Interrupt Enable
 
    - Fault event - not relevant
    
    
* constexpr functions has to be implemented in the header, cuz they are implicitly inline. The trick is to be 
able to evaluate a function at compile time, you need it a compile time and (before Linking). This means every 
translation unit has to have its own copy of this function implementation ready (at compile time) and can not wait for
the linker to find the actual implementation. 
 