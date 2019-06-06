--�������ID
--[[
6000214 [��]��ͭ����
3304163 �ͼ��ػ�ʯ
6000000 ��ţ���ѡ֮�ˣ�4Сʱ��
6000213 [��]�ҷ��ʯ
6000215 [��]��������
6000001 ��ţ���ѡ֮�ˣ�1�죩
6000100 ��â��ħ��-��
6000101 ����������-��
6000102 ��������-��
6000103 ����Ǭ����-��
6000104 �һ�����ӡ-��
6000105 ����������-��
6000106 ����������-��
6000107 ����������-��
3304164 �м��ػ�ʯ
6000002 ��ţ���ѡ֮�ˣ�3�죩
]]

--[[function i6000214_CanUse(MapID,InstanceID,TypeID,RoleID)

	local bRet,bIgnore = 0,false

	local FreeSize = role.GetBagFreeSize(RoleID)

	if FreeSize < 3 then

		bRet = 40
        end
	return bRet,bIgnore

end]]--

function i6000214_OnUse(MapID,InstanceID,TypeID,RoleID)


	local a = 3304163
	local b = 6000000
	local c = 6000213
	local a1 = 5
	local b1 = 1
	local c1 = 50

	role.AddRoleItem(MapID,InstanceID,RoleID,a,a1,-1,8,420)
	role.AddRoleItem(MapID,InstanceID,RoleID,b,b1,-1,8,420)
	role.AddRoleItem(MapID,InstanceID,RoleID,c,c1,-1,8,420)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 4128)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 4, 6000214)
	msg.AddMsgEvent(MsgID, 12, a1)
	msg.AddMsgEvent(MsgID, 4, a)
	msg.AddMsgEvent(MsgID, 12, b1)
	msg.AddMsgEvent(MsgID, 4, b)
	msg.AddMsgEvent(MsgID, 12, c1)
	msg.AddMsgEvent(MsgID, 4, c)
	msg.DispatchWorldMsgEvent(MsgID)
end


aux.RegisterItemEvent(6000214,1,"i6000214_OnUse")
--aux.RegisterItemEvent(6000214,0,"i6000214_CanUse")




--[[function i6000215_CanUse(MapID,InstanceID,TypeID,RoleID)

	local bRet,bIgnore = 0,false

	local FreeSize = role.GetBagFreeSize(RoleID)

	if FreeSize < 3 then

		bRet = 40

	end

	return bRet,bIgnore
end]]--



function i6000215_OnUse(MapID,InstanceID,TypeID,RoleID)

	local a = 3304163
	local b = 6000001
	local c = 6000213
	local a1 = 10
	local b1 = 1
	local c1 = 75

	role.AddRoleItem(MapID,InstanceID,RoleID,a,a1,-1,8,420)
	role.AddRoleItem(MapID,InstanceID,RoleID,b,b1,-1,8,420)
	role.AddRoleItem(MapID,InstanceID,RoleID,c,c1,-1,8,420)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 4128)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 4, 6000215)
	msg.AddMsgEvent(MsgID, 12, a1)
	msg.AddMsgEvent(MsgID, 4, a)
	msg.AddMsgEvent(MsgID, 12, b1)
	msg.AddMsgEvent(MsgID, 4, b)
	msg.AddMsgEvent(MsgID, 12, c1)
	msg.AddMsgEvent(MsgID, 4, c)
	msg.DispatchWorldMsgEvent(MsgID)
end

aux.RegisterItemEvent(6000215,1,"i6000215_OnUse")
--aux.RegisterItemEvent(6000215,0,"i6000215_CanUse")





--[[function i6000216_CanUse(MapID,InstanceID,TypeID,RoleID)

	local bRet,bIgnore = 0,false

	local FreeSize = role.GetBagFreeSize(RoleID)

	if(FreeSize < 5) then

		bRet = 40

	end
	return bRet,bIgnore
end]]--


function i6000216_OnUse(MapID,InstanceID,TypeID,RoleID)

	local EquipID = {}
        EquipID[1] = 6000100
	EquipID[2] = 6000101
	EquipID[3] = 6000102
	EquipID[4] = 6000103
	EquipID[5] = 6000104
	EquipID[6] = 6000105
	EquipID[7] = 6000106
        EquipID[8] = 6000107
	local a = 3304164
	local b = 6000002
	local c = 6000213
	local a1 = 10
	local b1 = 1
	local c1 = 175

 	local k = math.random(1,8)
        role.AddRoleItem(MapID,InstanceID,RoleID,EquipID[k],1,-1,8,420)
	local equip = EquipID[k]
        table.remove(EquipID,k)
        local j = math.random(1,7)
        role.AddRoleItem(MapID,InstanceID,RoleID,EquipID[j],1,-1,8,420)
	role.AddRoleItem(MapID,InstanceID,RoleID,a,a1,-1,8,420)
	role.AddRoleItem(MapID,InstanceID,RoleID,b,b1,-1,8,420)
	role.AddRoleItem(MapID,InstanceID,RoleID,c,c1,-1,8,420)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 4129)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 4, 6000216)
	msg.AddMsgEvent(MsgID, 12, 1)
	msg.AddMsgEvent(MsgID, 4, equip) 
	msg.AddMsgEvent(MsgID, 12, 1)
	msg.AddMsgEvent(MsgID, 4, EquipID[j])
	msg.AddMsgEvent(MsgID, 12, a1)
	msg.AddMsgEvent(MsgID, 4, a)
	msg.AddMsgEvent(MsgID, 12, b1)
	msg.AddMsgEvent(MsgID, 4, b)
	msg.AddMsgEvent(MsgID, 12, c1)
	msg.AddMsgEvent(MsgID, 4, c)
	msg.DispatchWorldMsgEvent(MsgID)

end


aux.RegisterItemEvent(6000216,1,"i6000216_OnUse")
--aux.RegisterItemEvent(6000216,0,"i6000216_CanUse")
