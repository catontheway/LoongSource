--[[����ID��14508
ʹ����Ʒˢ������
��ƷID��2614257
����ID��1007229
���꣺����̨�����Ѩ��943��13799��1214���������꣩]]


--��Ʒʹ��Ч���ű�����
function I2614258_QuestUsable(MapID, InstanceID, TypeID, TargetID)
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 1007229, x+2, y, z+2)
end

--��Ʒʹ���жϽű�����
function I2614258_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--�����ڷ���ʹ��
	if MapID ~= 1146339967 then
	    bRet = 43
	else
		local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
        if x<=943-20 or x>=943+20 or z<=1214-20 or z>=1214+20 then  --�����ڸ÷�Χ��ʹ��
            bRet = 43
        end
    end

	--����
	return bRet, bIgnore
end

--ע��
aux.RegisterItemEvent(2614258, 1, "I2614258_QuestUsable")
aux.RegisterItemEvent(2614258, 0, "I2614258_CanUse")
