--RKL147
--����1��ʹ����������2615047ˢ������ˮħ1009059

--�ɷ�ʹ�ú���
function i2615047_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	--����������أ�����ʹ��
	if MapID ~= 3017299407 then
	    bRet = 43
	else
	--�������ָ���ص㣬����ʹ��
	    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x < 2810 or x > 2910 or z < 1530 or z > 1630 then
	          bRet = 43 
            end
	end
    return bRet, bIgnore
end

--ʹ��Ч��
function i2615047_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       map.MapCreateCreature(MapID, InstanceID, 1009059, 2865, 7423, 1582)
end

--ע��
aux.RegisterItemEvent(2615047, 1, "i2615047_QuestUsable")
aux.RegisterItemEvent(2615047, 0, "i2615047_CanUse")