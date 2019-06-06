--��ǰ�����������12802
--����1������������ʯ����ʹ����Ʒ���ʯҼ[2612701]�ɷֱ�ˢ�³�����͵������[1005501]����������[1005502]����������[1005503]
--���꣺
--[m04: 2663, 2435]
--[m04: 2063, 1979]
--[m04: 1811, 2514]

--��Ʒ���ʯҼʹ���жϺ���
function i2612701_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false

	--������¹����ʹ��
	if MapID ~= 3017299663 then
	    bRet = 43
	else
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
        --����Ƿ�����Щ�㸽��
        if x>2647 and x<2679 and z>2419 and z<2451 then
            bRet = 0
        elseif x>2047 and x<2079 and z>1963 and z<1995 then
            bRet = 0
        elseif x>1795 and x<1827 and z>2498 and z<2530 then
            bRet = 0
        else
            bRet = 43
        end
    end

    --����
	return bRet, bIgnore
end

--��Ʒ���ʯҼʹ��Ч������
function i2612701_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)

	if x>2647 and x<2679 and z>2419 and z<2451 then
		map.MapCreateCreature(MapID, InstanceID, 1005501, 2663, 20783, 2435)
	elseif x>2047 and x<2079 and z>1963 and z<1995 then
		map.MapCreateCreature(MapID, InstanceID, 1005502, 2063,13898, 1979)
	elseif x>1795 and x<1827 and z>2498 and z<2530 then
		map.MapCreateCreature(MapID, InstanceID, 1005503, 1811, 12354, 2514)
	end
end

--ע��
aux.RegisterItemEvent(2612701, 1, "i2612701_QuestUsable")
aux.RegisterItemEvent(2612701, 0, "i2612701_CanUse")
