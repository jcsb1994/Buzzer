# Buzzer
High level interface for buzzers and beepers

Lets the user operate a buzzer from an ISR to remove the need for active wait periods

no more delay() while playing a melody with a buzzer!
Create custom melodies with the melody_t and pass them to your Buzzer object.
Everything will play automatically from an ISR with a single function call.