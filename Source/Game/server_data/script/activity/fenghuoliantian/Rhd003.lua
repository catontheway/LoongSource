
--ͳ�Ʒ�������н�����α�鱨��������
function x20003_Oncomplete(MapID, InstanceID, QuestID, OwnerID, NPCID)
	--�ۼӡ�α�鱨��������
	Act4_Gan_Num = Act4_Gan_Num + 1

	--��������α�鱨���������ﵽ50��ʱ���������յгɹ����¼�
	if Act4_Gan_Num==50 then
		--��ʾȫ����ҡ��յ�֮�Ƴɹ�������������Ѿۼ�������Ұ�⣡��ҿɾ������𣡡�
		local MsgInfoID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgInfoID, 100, 15)
		msg.DispatchMapMsgEvent(MsgInfoID, MapID, InstanceID)

		--�����¼���ˢ�³�����40ֻ������Ԯ����
		for i=1, 40 do
			local randPoint = math.random(1, 100)
			local x = map_list[3017298127].map[randPoint].x
			local y = map_list[3017298127].map[randPoint].y
			local z = map_list[3017298127].map[randPoint].z

			--ˢ����Ӧ����
			map.MapCreateCreature(3017298127, -1, 1003163, x, y, z)
		end
	end
	--���ӽ���ʵ��ֵ
	Increase_shili(MapID, InstanceID, OwnerID, 0, 1, "jinjieshili_A")

end

--��¼������Ԯ��������ɱ��
function c1003163_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	Act4_Dead_Creature_Num = Act4_Dead_Creature_Num + 1

	--��������Ԯ�����ı���ɱ���ﵽ35ֻʱ����������ָ���ĵ�����ˢ��BOSS���������ͳ˧��
	if Act4_Dead_Creature_Num == 35 then
		--��ʾ��������ͳ˧��####��####�����꣩���֣���ҿ�Э����֮����
		local randPoint = math.random(131, 139)
		local x = map_list[3017298127].map[randPoint].x
		local y = map_list[3017298127].map[randPoint].y
		local z = map_list[3017298127].map[randPoint].z

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 16)
		msg.AddMsgEvent(MsgID, 9, x)
		msg.AddMsgEvent(MsgID, 9, z)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

		--ˢ��������ͳ˧"
		map.MapCreateCreature(MapID, InstanceID, 1003164, x, y, z)
	end
end

--��������ͳ˧������ɱ�󣬷��������¿�ʼ��¼��α�鱨����������Ĵ����Լ�������Ԯ��������ɱ����
function c1003164_OnDie(MapID, InstanceID, CreatureID, TypeID, KillerID)
	--��ʼ���յ����������н�����α�鱨����
	Act4_Gan_Num = 0
	--��ʼ������Ԯ������ɱ��
	Act4_Dead_Creature_Num = 0
end





--aux.RegisterQuestEvent(20003, 1, "x20003_Oncomplete")
--aux.RegisterCreatureEvent(1003163, 4, "c1003163_OnDie")
--aux.RegisterCreatureEvent(1003164, 4, "c1003164_OnDie")
