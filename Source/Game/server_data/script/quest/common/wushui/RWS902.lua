--RZL902
--�ڷ��裨3017298383����2096��2472��X,Z��������x��z����70��ʹ�û����飨3200518������2058��10842��2431ˢ�����ݻ���1531002��

--�ɷ�ʹ�ú���
function i3200518_CanUse(MapID, InstanceID, TypeID, TargetID)
    local bRet, bIgnore = 0, false

	--������ڷ��賡��������ʹ��
	if MapID ~= 3017298383 then
	    bRet = 43
	else
	    --�õ���ҵ�ǰ����
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x<=2026 or x>=2166 or z<=2402 or z>=2542 then  --�������������귶Χ������ʹ��
	        bRet = 43
	    end
	end

	--����
	return bRet, bIgnore
end

--������ʹ��Ч��
function i3200518_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 1531002, 2058, 10842, 2431)
end

--ע��
aux.RegisterItemEvent(3200518, 1, "i3200518_QuestUsable")
aux.RegisterItemEvent(3200518, 0, "i3200518_CanUse")
