/* Ölfontaine */

#strict

local derr;

func Set(object tar) {
  derr = tar;
  }

func Spritz(){
  var i, flame;
  var y = RandomX(-50, -30);

  if(!derr->Burning())
    for(i = 1; i < 5; i++)
      CreateParticle("Oil",0,0,RandomX(-5,5),y,20,RGBa(20,20,20,0));
    else
      if(!Random(4)) {
        flame = CreateObject(DFLM);
	flame->SetSpeed(RandomX(5, -5), RandomX(-30, -40));
	}
  return(1);
}
