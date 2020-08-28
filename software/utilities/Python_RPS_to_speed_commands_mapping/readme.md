### RPS to Speed Command Mapping

This python implementation creates a const time look up table for speed commands (ranging from 0 to 100)  and the RPS that corresponds to it. It will  be used by feed-forward PID to get the approximate open loop value command value for a target RPS.

Currently, this is done with a mock list of RPS and speed command pairs. The actual implementation can should read it from a .txt file that is created from the Teensy Serial output. There is a function on Teensy code that outputs the real RPS-Speed Command pairs accordingly. The only thing to do is copy paste the data from the terminal or pipe it to a txt file.

The created table should be included in a header file on Teensy code.