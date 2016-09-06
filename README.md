# sgrigliatore
FarieCasari
Progetto di un Sgrigliatore a catena per la pulizia di una piccola turbina da 5KW situata in una roggia con 1M di salto

Nasce dalla necessità di pulire uno sgrigliatore a catena, in commercio non si trova un timer con due tempi variabili 
e qualche sicurezza allora ho deciso di usare Arduino in realtà Leonardo mini.
Il progetto consiste nel fare Tre giri della catena ogni X tempo settabile a seconda del esigenga d'estate e inverno 
qualche ora mentre in primavera ed autunno ogni 15 minuti
con una sicuressa che se il sensore non viene letto dopo 3 minuti tutto si ferma, nel caso si incastri qualche cosa 
nella griglia almeno non si brucia il motore


è composto da arduino Leonardo nano
lcd i2c
encoder rotativo
pulsante 
due relè
due opto 
un sensore induttivo
