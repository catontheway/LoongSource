--RKL017
--ʹ�ô�����������[2615003]��ˢ�� ����[1009003]
--2423-70<x<2423+70,2374-70<z<2374+70

--�ɷ�ʹ�ú���
function i2615003_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	--����������أ�����ʹ��
	if MapID ~= 3017299407 then
	    bRet = 43
	else
	--�������ָ���ص㣬����ʹ��
	    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x < 2353 or x > 2493 or z < 2304 or z > 2444 then
	          bRet = 43
            end
	end
    return bRet, bIgnore
end

--ʹ��Ч��
function i2615003_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
       map.MapCreateCreature(MapID, InstanceID, 1009003, x, y, z)
end

--ע��
aux.RegisterItemEvent(2615003, 1, "i2615003_QuestUsable")
aux.RegisterItemEvent(2615003, 0, "i2615003_CanUse")
