
----RWY009 ս��֮��[�ظ�]
--����1����NPC�����Ի���ȡ�������Ҵ���������ս��� 13412/50,1143,22267/50
--����2�����븱�����BUFFս��ף��������ʱ��10����
--����4: ��������뿪����������

--����1����NPC�����Ի���ȡ�������Ҵ���������ս��� 13412/50,1143,22267/50
--����2����NPC�����Ի���ȡ�������BUFFս��ף��������ʱ��10����
function q14009_OnAccept(mapID, instanceID, questID, ownerID, accepterID)
	role.RoleGotoNewMap(mapID, instanceID, ownerID, 1826991143, 260, 1201, 460)
end

--����4: ��������뿪����������
function q14009_OnComplete(MapID, InstanceID, QuestID, OwnerID, NPCID)
	role.RoleGotoNewMap(MapID, InstanceID, OwnerID, 3017299151, 957, 20140, 848)
end

aux.RegisterQuestEvent(14009, 0, "q14009_OnAccept")
aux.RegisterQuestEvent(14009, 1,"q14009_OnComplete")


--���븱����һ��һ��buff ս��ף��
function q07_OnPlayerEnter(MapID, InstanceID,RoleID)
	 unit.AddBuff(MapID, InstanceID,RoleID, 2015801,RoleID)
end
--ע��
aux.RegisterMapEvent("q07", 2, "q07_OnPlayerEnter")
