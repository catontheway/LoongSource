--����ȼ�վ���
function x20002_OnAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	--���һ��ˢ�ֵ�
	local randPoint = math.random(101, 130)
	local x = map_list[3017298127].map[randPoint].x
	local y = map_list[3017298127].map[randPoint].y
	local z = map_list[3017298127].map[randPoint].z

	--����һ����������������⡱
	map.MapCreateCreature(3017298127, -1, 3010225, x, y, z)

	--������ˢ�³�2-4�������廤��ʹ��
	local Num = math.random(2, 4)
	for k=1, Num do
		map.MapCreateCreature(3017298127, -1, 1003162, x+1, y, z+1)
	end

	--ȫ������ʾ��������һ���о�������! λ��####��####(����)!��
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 10)
	msg.AddMsgEvent(MsgID, 9, x)
	msg.AddMsgEvent(MsgID, 9, z)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end


function x20002_OnInvest(MapID, InstanceID, QuestID, OwnerID, NPCID)
	--ȫ�������ǳ������ͨ�棺��###��������ƣ��ɹ��ջ�һ�����������!��
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 11)
	msg.AddMsgEvent(MsgID, 2, OwnerID)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

	--25%���ʻ��һ��������ߡ����������
	local Rate = math.random(1, 100)
	if Rate <=25 then
		role.QuestAddRoleItem(MapID, InstanceID, OwnerID, QuestID, 2610161, 1, -1, 101)
	end

	--�������
	role.RoleCompleteQuest(OwnerID, QuestID, -1)
		--���ӽ���ʵ��ֵ
	Increase_shili(MapID, InstanceID, OwnerID, 0, 1, "jinjieshili_A")

end

--aux.RegisterQuestEvent(20002, 0, "x20002_OnAccept")
--aux.RegisterQuestEvent(20002, 9, "x20002_OnInvest")
