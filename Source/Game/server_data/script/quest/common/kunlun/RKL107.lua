--RKL107
--����1��ʹ��������Ѫ2615029���ˢ������1009033

--�ɷ�ʹ�ú���
function i2615029_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false
	--����������أ�����ʹ��
	if MapID ~= 3017299407 then
	    bRet = 43
	else
	--�������ָ���ص㣬����ʹ��
	    local x,y,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x < 2300 or x > 2400 or z < 1580 or z > 1680 then
	          bRet = 43 
            end
	end
    return bRet, bIgnore
end

--ʹ��Ч��
function i2615029_QuestUsable(MapID, InstanceID, TypeID, TargetID)
       map.MapCreateCreature(MapID, InstanceID, 1009033, 2348, 7441, 1628)
end

--ע��
aux.RegisterItemEvent(2615029, 1, "i2615029_QuestUsable")
aux.RegisterItemEvent(2615029, 0, "i2615029_CanUse")