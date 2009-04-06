/*-- Fontäne --*/

#strict

local width,height,t1,t2,state,max;

func Initialize()
{
  height = 1;
  Set(GetDefWidth(),80);
  return(1);
}

func Set(int iWidth, int iMax)
{
  width = iWidth;
  max = iMax;
}

func Timer()
{
  height = Abs(Sin(t1,max));
  Draw();

  if(state == 0)
    return(Rise());
    
  if(state == 1)
    return(Erupt());
    
  if(state == 2)
    return(Collapse());
}

func Rise()
{
  if(!Random(30))
    Sound("Lava*");
  
  t1+=2;
  width = Abs(Sin(t1,max/2));
  
  if(!Random(2))
    CastParticles ("LavaGlow2", 1+Random(2),5, 0, -(height-5), 10, 50);
  
  if(height == max)
    state = 1;
}

func Erupt()
{
  t1++;
  width = Abs(Sin(t1,max/2));

  if(!Random(5))
    CastObjectsX(LAVA,1,height + Random(40),0,-(height-5),270,40);
    
  CreateParticle ("LavaGlow3", 0, -(height-5), Random(6)-3, -(height/2 + Random(50)), 30+Random(50));
    
  Sound("Discharge");
  
  t2++;
  if(t2++ > 35*3)
    state = 2;
}

func Collapse()
{
  if(!Random(3))
    CastParticles ("LavaGlow2", 1+Random(2),5, 0, -(height-5), 10, 50);
  
  t1+=4;
  
  if(height <= 7)
    RemoveObject();
}

func Draw()
{
  SetObjDrawTransform
  (
    width*1000/GetDefWidth(), 0, 0,
    0, height*1000/GetDefHeight(),(GetDefHeight()/2-height/2)*1000
  );
}