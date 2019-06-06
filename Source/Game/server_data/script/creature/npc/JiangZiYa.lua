--NPC���㴫���˽ű�

function JiangZiYa_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if rolelevel < 20 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 282)	-- "�������ڣ�����Ϫ�ߣ�ɽҰ���ˣ������ס���"
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if MapID== 2983744207 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 276)		--����������ʿ����ȥ�����ӣ��Ƿ��Ѿ���ᵽ�رߴ����������Ե�����أ���ʿ��Ҫ�ص��������ǣ�����֪���Ϸ򡣡�  ��m20
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
				msg.AddMsgEvent(MsgID, 1,	279)		--������Ҳ����һ�����̣�
				msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	281)		--���������ˡ�����
				msg.AddMsgEvent(MsgID, 21,	6)			--��ѯѡ��
				msg.AddMsgEvent(MsgID, 1,	285)		--����ѯ���������ȡ�
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

			elseif MapID== 3017298127 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 277)		--����ʿ����ƣ�룬�����ܾ���ɳ����ս���Ͷ١��β�ѧѧ�Ϸ�ƫ��һ�磬ɽˮ֮�������֮�֡�����ͬ��սɳ��һ���ɻ�þ���������Ҳ�����ҵ�ʲô�����챦Ҳδ��֪������  ��m02
				msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
				msg.AddMsgEvent(MsgID, 21,	4)			--ȷ����ť
				msg.AddMsgEvent(MsgID, 1,	278)		--����Ҫ�ص���������
				msg.AddMsgEvent(MsgID, 21,	5)			--ȡ����ť
				msg.AddMsgEvent(MsgID, 1,	281)		--���������ˡ�����
				msg.AddMsgEvent(MsgID, 21,	6)			--��ѯѡ��
				msg.AddMsgEvent(MsgID, 1,	285)		--����ѯ���������ȡ�
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	elseif TalkIndex == 4 then
		local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)

		if MapID== 2983744207 and rolelevel>=20 then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3017298127, 2214, 5738, 2132)
		elseif MapID== 3017298127 and rolelevel>=20 then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2983744207, 964, 10095, 2001)
		end
		TalkIndex = -1
	elseif TalkIndex == 6 then
		local a = role.GetRoleScriptData(RoleID, 1, RoleDataType["FishingLevel"])
		local b = role.GetRoleScriptData(RoleID, 1, RoleDataType["FishingNum"])
		if a == 0 then
			a = 1
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 286) --���ĵ��������ȵȼ�Ϊ<p1>,����<p2>��������
		msg.AddMsgEvent(MsgID, 9, a)
		msg.AddMsgEvent(MsgID, 9, a*50-b)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		TalkIndex = -1
	end
end

-- ע��
aux.RegisterCreatureEvent(3000023, 7, "JiangZiYa_OnTalk")

