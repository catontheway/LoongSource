--ʹ�ü��ܺ���

function s2311702_Cast(mapid, instanceid, skillid, ownerid)
     local TypeID = cre.GetCreatureTypeID(mapid,instanceid, ownerid)
     if TypeID == 1004531 then
     local temp = math.random(1,100)
	    if temp>=1 and temp<=20 then
		    --20%���ʺ���
                 cre.MonsterSay(mapid, instanceid, ownerid, 10071)
           end
     elseif TypeID == 1004533 then
     local temp = math.random(1,100)
	    if temp>=1 and temp<=20 then
		    --20%���ʺ���
                 cre.MonsterSay(mapid, instanceid, ownerid, 10075)
           end
     end
     return 0
end


--ע��

aux.RegisterSkillEvent(2311702, 1, "s2311702_Cast")

