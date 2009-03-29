/*-- Rampe --*/


#strict



local dir;



public func Left(string szMaterial)

{

  dir = 0;
  Set(szMaterial);

  if(FrameCounter() > 10)

  {

    Log("DrawMaterialQuad(%s,%d,%d,%d,%d,%d,%d,%d,%d,true);",szMaterial,GetX()+15,GetY(),GetX()-15,GetY()-10,GetX()-15,GetY(),GetX(),GetY());

  }

}



public func Right(string szMaterial)

{

  dir = 1;
  Set(szMaterial);

  if(FrameCounter() > 10)

  {

    Log("DrawMaterialQuad(%s,%d,%d,%d,%d,%d,%d,%d,%d,true);",szMaterial,GetX()-15,GetY(),GetX()+15,GetY()-10,GetX()+15,GetY(),GetX(),GetY());

  }

}



protected func Set(string szMaterial)

{

  if(FrameCounter() > 10) Sound("Connect",true);

  
  if(dir)//Rechts...

    DrawMaterialQuad (szMaterial, GetX()-15, GetY(),GetX()+15,GetY()-10, GetX()+15, GetY(), GetX(), GetY(), true);

  else//Links...

    DrawMaterialQuad (szMaterial, GetX()+15, GetY(),GetX()-15,GetY()-10, GetX()-15, GetY(), GetX(), GetY(), true);

    
  RemoveObject();

}