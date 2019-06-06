--�������ڣ�13803
--����1��������ɽ��[m05: 1991, 1396]����ʹ����Ʒ��������[2613606]��ˢ�³���������ѲɽС��[1006607]
--����2��������ɽС��[1006607]������ˢ�³�������ʹ[1006608]
--����3��������ʹ[1006608]������ˢ�³���������[1006609]�������ҷ���[1006610]
--����4����������[1006609]������ˢ�³���������[1006611]

--��Ʒ��������ʹ���ж�
function i2613606_CanUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false

	--������ˮ����ʹ��
	if MapID ~= 3017299919 then
	    bRet = 43
	else
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	    if x<=1975 or x>=2007 or z<=1380 or z>=1412 then    --���ڸ�λ�ò���ʹ��
	        bRet = 43
	    end
	end

	--����
	return bRet, bIgnore
end

--��Ʒ��������ʹ��Ч������
function i2613606_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 1006607, 1991, 24790, 1396)
end

--ע��
aux.RegisterItemEvent(2613606, 1, "i2613606_QuestUsable")
aux.RegisterItemEvent(2613606, 0, "i2613606_CanUse")

--��������ѲɽС����������
function c1006607_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1006608, 1991, 24790, 1396)
end

--����������ʹ��������
function c1006608_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	map.MapCreateCreature(MapID, InstanceID, 1006609, 1988, 24790, 1396)
	map.MapCreateCreature(MapID, InstanceID, 1006610, 1994, 24790, 1396)
end

--��������������������
function c1006609_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    cre.MonsterSay(MapID, InstanceID, TargetID, 30122)
	map.MapCreateCreature(MapID, InstanceID, 1006611, 1991, 24790, 1396)
end

--ע��
aux.RegisterCreatureEvent(1006607, 4, "c1006607_OnDie")
aux.RegisterCreatureEvent(1006608, 4, "c1006608_OnDie")
aux.RegisterCreatureEvent(1006609, 4, "c1006609_OnDie")
