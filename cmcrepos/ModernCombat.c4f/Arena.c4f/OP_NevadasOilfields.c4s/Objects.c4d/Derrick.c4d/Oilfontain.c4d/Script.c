/* Ölfontaine */

#strict

func Spritz(){
var i;
var y=RandomX(-50,-30);
for(i=1;i<5;i++)
CreateParticle("Oil",0,0,RandomX(-5,5),y,20,RGBa(20,20,20,0));
return(1);
}
