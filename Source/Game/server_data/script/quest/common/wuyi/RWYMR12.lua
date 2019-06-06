
--RWYMR12���������� 14312
--ʹ��[��ǩ][2614037]��ɾ��һ��[δ����Ķ�Һ][2614038]����40%���ʻ��[��ɫ��Һ][2614039]��40%���ʻ��[��ɫ��Һ][2614040]�����������20%���ʻ��[���Զ�Һ][2614041]
function I2614037_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	--ɾ��һ��[δ����Ķ�Һ][2614038]
	role.RemoveFromRole(MapID, InstanceID, TargetID, 2614038, 1, 101)

	local Rate = math.random(1, 100)
	if Rate < 51 then
	--20%���ʻ��[���Զ�Һ][2614041]
	role.AddRoleItem(MapID, InstanceID, TargetID, 2614041, 1, -1, 8,420)

	--40%���ʻ��[��ɫ��Һ][2614040]
	elseif Rate >= 51 and Rate < 75 then
	role.AddRoleItem(MapID, InstanceID, TargetID, 2614040, 1, -1, 8,420)

	--40%���ʻ��[��ɫ��Һ][2614039]
	elseif Rate >= 75 and Rate <= 100 then
	role.AddRoleItem(MapID, InstanceID, TargetID, 2614039, 1, -1, 8,420)

	end
end

--��������һ��[δ����Ķ�Һ][2614038]����ǩ�ſ�ʹ��
function I2614037_CanUse(MapID, InstanceID, TypeID, TargetID)
	local num = role.GetRoleItemNum(TargetID, 2614038)

	local bRet, bIgnore = 0, false

	if num < 1 then
	bRet = 32
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(2614037, 1, "I2614037_QuestUsable")
aux.RegisterItemEvent(2614037, 0, "I2614037_CanUse")

--ʹ��[��ɫ��Һ][2614040]�������Ѫ��BUFF[2016101]
function I2614040_Use(MapID, InstanceID, TypeID, TargetID)
	unit.AddBuff(MapID, InstanceID, TargetID, 2016101, TargetID)
end
--ʹ��[��ɫ��Һ][2614039]����ÿ񱩶�BUFF[2016001]
function I2614039_Use(MapID, InstanceID, TypeID, TargetID)
	unit.AddBuff(MapID, InstanceID, TargetID, 2016001, TargetID)
end
aux.RegisterItemEvent(2614040,1, "I2614040_Use")
aux.RegisterItemEvent(2614039,1, "I2614039_Use")
