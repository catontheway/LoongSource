--ʹ�ü��ܺ���

function s2307201_Cast(mapid, instanceid, skillid, ownerid)
     local TypeID = cre.GetCreatureTypeID(mapid,instanceid, ownerid)
     if TypeID == 1004426 then
     local temp = math.random(1,100)
	    if temp>=1 and temp<=20 then
		    --20%���ʺ���
                 cre.MonsterSay(mapid, instanceid, ownerid, 10088)
           end
     end
     return 0
end


--ע��

aux.RegisterSkillEvent(2307201, 1, "s2307201_Cast")
