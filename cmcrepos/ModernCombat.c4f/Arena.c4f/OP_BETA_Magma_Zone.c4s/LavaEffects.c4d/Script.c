/*-- LavaEffekte --*/

#strict

public func Initialize()
{
  SetPosition();
}

public func Timer()
{
  if(!Random(10))
  {
    DoRndLavaJet();//Allgemeine Funktion.
    //DoLavaJet(Random(LandscapeWidth()),LandscapeHeight()-2);
  }
  
  if(!Random(2))
  {
    DoRndLavaBubble();//Allgemeine Funktion.
    //DoLavaBubble(Random(LandscapeWidth()),LandscapeHeight()-2);
  }
}


//LavaJet
private func DoLavaJet(int x, int y)
{
  //Oberfläche.
  while(CheckLava(x,y))
  {
    y--;
  }
    
  var obj = CreateObject(_LFN,x,y+3,NO_OWNER);
  obj->Set(30+Random(50),50+Random(100));
  return(obj);
}

private func DoRndLavaJet()
{
  var x,y;
  if(!FindRndLavaPoint(x,y)) return(0);
  return(DoLavaJet(x,y));
}

//Pustefix?!
private func DoLavaBubble(int x, int y)
{
  //Grund.
  while(CheckLava(x,y))
    y++;//Müsste sich eigentlich aufhängen? :O
    
  return(CreateObject(_LBB,x,y,NO_OWNER));
}

private func DoRndLavaBubble()
{
  var x,y;
  if(!FindRndLavaPoint(x,y)) return(0);
  return(DoLavaBubble(x,y));
}


//Helperz
private func CheckLava(int x, int y)
{
  if(GetMaterial(x,y) == Material("DuroLava"))
    return(true);
  return(false);
}

private func FindRndLavaPoint(int & x, int & y)
{
  //Zufälligen Punkt mit Lava finden.
  for(var i = 10; i > 0; i--)
  {
    x = Random(LandscapeWidth());
    y = Random(LandscapeWidth());
    
    if(CheckLava(x,y))
      break;
  }
  
  if(!x && !y) return(false);
  return(true);
}