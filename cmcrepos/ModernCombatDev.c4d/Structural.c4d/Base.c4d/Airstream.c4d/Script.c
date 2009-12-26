/*-- Windstrom --*/

#strict

local width,height,power;


/* Initalisierung */

func Initialize()
{
  Set(40,60,5);

  SetAction("Push");
  return(1);
}

func Set(w,h,p)
{
  width = w;
  height = h;
  power = p;
}

/* Schiebeeffekt */

func Push()
{
  for(var obj in FindObjects(Find_InRect(-(width/2),-height,width/2,height),Find_Or(Find_Category(C4D_Vehicle),Find_OCF(OCF_Collectible),Find_OCF(OCF_Living))))//links
  {
    if(obj->~IsClonk())
      if(GetProcedure(obj) eq "WALK")
        obj->SetAction("Jump");
        
    SetXDir(GetXDir(obj,100)-1,obj,100);
    SetYDir(GetYDir(obj)-power,obj);
  }

  for(var obj in FindObjects(Find_InRect(0,-height,width/2,height),Find_Or(Find_Category(C4D_Vehicle),Find_OCF(OCF_Collectible),Find_OCF(OCF_Living))))//rechts
  {
    if(obj->~IsClonk())
      if(GetProcedure(obj) eq "WALK")
        obj->SetAction("Jump");
        
    SetXDir(GetXDir(obj,100)+1,obj,100);
    SetYDir(GetYDir(obj)-power,obj);
  }
	
  for(var i = 3; i > 0; i--)
    CreateParticle ("Air",RandomX(-(width/2),(width/2)),-2,0,-(power*2),20+Random(80),RGBa(0,Random(100),255),this(),true);
}