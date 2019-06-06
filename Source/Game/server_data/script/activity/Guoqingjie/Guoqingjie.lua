

--1006730	���ʿ���
--3610223	������
--RoleDataType["guoqing_tongmingguiyu"]   = 152  ��¼�������ҽ���ͨڤ���򸱱������������Ӵ���
--RoleDataType["Tongmingguiyu_wuzixiang"]   = 156  --��¼�������ҽ���ͨڤ���򸱱�����

--�Ի����ʿ⣬ˢ�����ʿ���
function Wuzixiang_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local  Level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local  x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 4009)	-- "װ��ս�����ʵ����ӣ������������ƺ���Щ�쳣��"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��
		msg.AddMsgEvent(MsgID, 1, 4010)	-- "���ŵ���һ�£��ٻ����ʿ��أ�"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

	if TalkIndex == 4 then  -- ���ŵ���һ�£��ٻ����ʿ��أ�
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 4012)  --���ʿ��ر��ٻ������ˣ���������ս����׼��
		msg.DispatchMapMsgEvent(MsgID, "m42", -1)
		map.MapCreateColCreature(MapID, InstanceID, 1006730, x, y, z, 1)
		map.MapDeleteCreature(MapID, InstanceID, Tongmingguiyu_wuzixiang) -- ɾ��������
	end
end
aux.RegisterCreatureEvent(3610223, 7, "Wuzixiang_OnTalk")



--��ɱ���ʿ��أ�����С�ӳ�Ա��ù������ʵ���

function c1006730_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	local Role = {}
	Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
	for i=1, 6 do
	    if Role[i] and Role[i]~= 4294967295 then
		    local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
			local k = math.random(2,6)
			if Target_MapID == MapID then
			    role.AddRoleItem(Target_MapID, Target_InstanceID, Role[i], 6000110, k, -1, 8, 420) --Ϊ�����е�С�ӳ�Աÿ�����2~6����������
			end
	    end
	end

end
--ע��
aux.RegisterCreatureEvent(1006730,4,"c1006730_OnDie")


--ʹ���ӱ���������̻���ս���ȵ���
--2431162	[��]�������
--2431322	[��]ɢ�鷨��


function I6000109_OnUse(MapID, InstanceID, TypeID, RoleID)
	local k = math.random(2)
	local l = math.random(2)
	if k == 1 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 6000111, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, RoleID, 6000112, 1, -1, 8, 420)
	end
	if l == 1 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 2431162, 1, -1, 8, 420)
	else
		role.AddRoleItem(MapID, InstanceID, RoleID, 2431322, 1, -1, 8, 420)
	end
end



function I6000109_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	--�жϱ������пռ��Ƿ��㹻
	local FreeSize = role.GetBagFreeSize(RoleID)
	if(FreeSize < 2) then
		--��ʾ��ұ����ռ䲻��
		bRet = 40
		return bRet, bIgnore
	end
end


--��NPC�ı��������ͷ��̻�����ø������

--9437701			����ͬ��
--9437801		�̻�
--9438001			�̻�



function I6000111_OnUse(MapID, InstanceID, TypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local a = map.MapCreateCreature(MapID, InstanceID, 4900601, x+2, y, z+2)
	unit.AddBuff(MapID, InstanceID, a, 2023201, a)
	unit.AddBuff(MapID, InstanceID, RoleID, 9437701, RoleID)
end



function I6000111_CanUse(MapID, InstanceID, TypeID, RoleID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	local t = map.IsInArea(3017299663, -1, RoleID, 10994)	--����true��false
	local FreeSize = role.GetBagFreeSize(RoleID)
	if Guoqingjie == 1 then
		if Npcyuebing ~= 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 4013) -- �����ı���δ��ʼ�����������ı���ʼ��վ���ı����鸽��ʹ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 32, false
		elseif t ~= true  then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 4014) -- ����վ�������ı����鸽���ſ���ʹ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 32, false
		elseif (FreeSize < 1) then
			return 40, false
		else
			return 0, false
		end
	else 							--������ڼ������ڹ̶�ʱ��͵ص㿪���������֮�������ơ�
		if (FreeSize < 1) then
			return 40, false
		else
			return 0, false
		end
	end
end

function I6000112_OnUse(MapID, InstanceID, TypeID, RoleID)

	local x, y, z = unit.GetPosition(MapID, InstanceID, RoleID)
	local a = map.MapCreateCreature(MapID, InstanceID, 4900601, x+2, y, z+2)
	unit.AddBuff(MapID, InstanceID, a, 2023401, a)
	unit.AddBuff(MapID, InstanceID, RoleID, 9437701, RoleID)
end



function I6000112_CanUse(MapID, InstanceID, TypeID, RoleID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	local t = map.IsInArea(3017299663, -1, RoleID, 10994)	--����true��false
	local FreeSize = role.GetBagFreeSize(RoleID)
	if Guoqingjie == 1 then
		if Npcyuebing ~= 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 4013) -- �����ı���δ��ʼ�����������ı���ʼ��վ���ı����鸽��ʹ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 32, false
		elseif t ~= true  then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 4014) -- ����վ�������ı����鸽���ſ���ʹ��
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 32, false
		elseif (FreeSize < 1) then
			return 40, false
		else
			return 0, false
		end
	else 							--������ڼ������ڹ̶�ʱ��͵ص㿪���������֮�������ơ�
		if (FreeSize < 1) then
			return 40, false
		else
			return 0, false
		end
	end
end



aux.RegisterItemEvent(6000109, 1, "I6000109_OnUse")
aux.RegisterItemEvent(6000111, 1, "I6000111_OnUse")
aux.RegisterItemEvent(6000112, 1, "I6000112_OnUse")
aux.RegisterItemEvent(6000109, 0, "I6000109_CanUse")
aux.RegisterItemEvent(6000111, 0, "I6000111_CanUse")
aux.RegisterItemEvent(6000112, 0, "I6000112_CanUse")



--�������
function x20330_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

    --[[local CurTime = tonumber(os.date("%j"))
	if role.GetRoleScriptData(RoleID, 1, RoleDataType["Guoqing_Date"]) ~= CurTime then
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["Guoqing_Date"], CurTime)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Guoqing_WuZi"], 0)
	end]]
	role.SetRoleScriptData(RoleID, 1, RoleDataType["Guoqing_WuZi"], role.GetRoleScriptData(RoleID, 1, RoleDataType["Guoqing_WuZi"])+1)

end

function x20330_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)

	local CurTime = tonumber(os.date("%j"))
	if role.GetRoleScriptData(RoleID, 1, RoleDataType["Guoqing_Date"]) ~= CurTime then
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["Guoqing_Date"], CurTime)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Guoqing_WuZi"], 0)
	end

	if role.GetRoleScriptData(RoleID, 1, RoleDataType["Guoqing_WuZi"]) >= 3 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 147)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    return 0
	else
	    return 1
	end

end

aux.RegisterQuestEvent(20330, 1, "x20330_OnComplete")
aux.RegisterQuestEvent(20330, 4, "x20330_OnCheckAccept")






































