
--
function Motion_OnUse(MapID,InstanceID,RoleID, DstRoleID, MotionID)
	if Qingrenjie == 1 then
		if MotionID==14  and role.GetRoleItemNum(RoleID,2616518) > 0 then
			local k = math.random(100)
			if k <= 25 then
				role.RemoveFromRole(MapID,InstanceID,RoleID,2616518,1,420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 2616519, 1, -1, 8, 420)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 12613)--ţ�ɵ�ף��״̬��ʧ������ٴλ�÷�ţ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		end
	end

	if act.GetActIsStart(151) then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
		for i = 1,4 do
			if m02_qixi_point[i].npc == 1 and (x < m02_qixi_point[i].x + 100 or x > m02_qixi_point[i].x - 100) and (z < m02_qixi_point[i].z + 100 or z > m02_qixi_point[i].z - 100) then
				local a = role.IsRoleHaveBuff(MapID,InstanceID,RoleID,50050)
				local b = role.IsRoleHaveBuff(MapID,InstanceID,DstRoleID,50050)
				if MotionID==14 then
					if a == false then
						unit.AddBuff(MapID, InstanceID, RoleID, 5005001, RoleID)
						role.AddRoleItem(MapID, InstanceID, RoleID, 3305001,1 , -1, 8, 420)
					else
						--��ʾ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 12140)--ţ�ɵ�ף��״̬��ʧ������ٴλ�÷�ţ��
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
					if b == false then
						unit.AddBuff(MapID, InstanceID, DstRoleID, 5005001, DstRoleID)
						role.AddRoleItem(MapID, InstanceID, DstRoleID, 3305001,1 , -1, 8, 420)
					else
						--��ʾ
					    local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 12140)
						msg.DispatchRoleMsgEvent(DstRoleID, MsgID)
					end
				end
			elseif m02_qixi_point[i].npc == 2 and (x < m02_qixi_point[i].x + 100 or x > m02_qixi_point[i].x - 100) and (z < m02_qixi_point[i].z + 100 or z > m02_qixi_point[i].z - 100) then
				local a = role.IsRoleHaveBuff(MapID,InstanceID,RoleID,50051)
				local b = role.IsRoleHaveBuff(MapID,InstanceID,DstRoleID,50051)
				if MotionID==14 then
					if a == false then
						unit.AddBuff(MapID, InstanceID, RoleID, 5005101, RoleID)
						role.AddRoleItem(MapID, InstanceID, RoleID, 3305002,1 , -1, 8, 420)
					else
						--��ʾ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 12141)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
					if b == false then
						unit.AddBuff(MapID, InstanceID, DstRoleID, 5005101, DstRoleID)
						role.AddRoleItem(MapID, InstanceID, DstRoleID, 3305002,1 , -1, 8, 420)
					else
						--��ʾ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 12141)
						msg.DispatchRoleMsgEvent(DstRoleID, MsgID)
					end
				end
			elseif m04_qixi_point[i].npc == 1 and (x < m04_qixi_point[i].x + 100 or x > m04_qixi_point[i].x - 100) and (z < m04_qixi_point[i].z + 100 or z > m04_qixi_point[i].z - 100) then
				local a = role.IsRoleHaveBuff(MapID,InstanceID,RoleID,50050)
				local b = role.IsRoleHaveBuff(MapID,InstanceID,DstRoleID,50050)
				if MotionID==14 then
					if a == false then
						unit.AddBuff(MapID, InstanceID, RoleID, 5005001, RoleID)
						role.AddRoleItem(MapID, InstanceID, RoleID, 3305001, 1, -1, 8, 420)
						else
						--��ʾ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 12140)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
					if b == false then
						unit.AddBuff(MapID, InstanceID, DstRoleID, 5005001, DstRoleID)
						role.AddRoleItem(MapID, InstanceID, DstRoleID, 3305001, 1, -1, 8, 420)
					else
						--��ʾ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 12140)
						msg.DispatchRoleMsgEvent(DstRoleID, MsgID)
					end
				end
			elseif m04_qixi_point[i].npc == 2 and (x < m04_qixi_point[i].x + 100 or x > m04_qixi_point[i].x - 100) and (z < m04_qixi_point[i].z + 100 or z > m04_qixi_point[i].z - 100) then
				local a = role.IsRoleHaveBuff(MapID,InstanceID,RoleID,50051)
				local b = role.IsRoleHaveBuff(MapID,InstanceID,DstRoleID,50051)
				if MotionID==14 then
					if a == false then
						unit.AddBuff(MapID, InstanceID, RoleID, 5005101, RoleID)
						role.AddRoleItem(MapID, InstanceID, RoleID, 3305002, 1, -1, 8, 420)
					else
						--��ʾ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 12141)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					end
					if b == false then
						unit.AddBuff(MapID, InstanceID, DstRoleID, 5005101, DstRoleID)
						role.AddRoleItem(MapID, InstanceID, DstRoleID, 3305002, 1, -1, 8, 420)
					else
						--��ʾ
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 12141)
						msg.DispatchRoleMsgEvent(DstRoleID, MsgID)
					end
				end
			end
		end
	end

	if MotionID==10 then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
		if MapID == 3017298127 and math.abs(x-1207)< 50 and math.abs(z-3664)< 50 and role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 20196) == true then
			role.RoleCompleteQuest(RoleID, 20196, -1)
		elseif MapID == 3017298383 and math.abs(x-1760)< 50 and math.abs(z-1833)< 50 and role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 20197) == true then
			role.RoleCompleteQuest(RoleID, 20197, -1)
		elseif MapID == 3017299663 and math.abs(x-1821)< 50 and math.abs(z-1851)< 50 and role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 20198) == true then
			role.RoleCompleteQuest(RoleID, 20198, -1)
		end
	end

--***************************************	 ����������������ʹ�ø��Զ���	******************************************************

	-- ��ˮ	�������� ObjID = 6307  --
	-- ��¹ �������� ObjID = 10994 --
	-- �Ϲ� 2
	-- ��Ц 6
	-- ���� 7
	-- ���� 10
	-- ���� 13
	--1535410	У���ӱ�	15
	--1535411	У���ӱ�	30
	--1535412	У���ӱ�	45
	--1535413	У���ӱ�	60
	--1535414	У���ӱ�	75
	--1535415	У���ӱ�	90
	--9437201				���Ƶȴ�
	--9437301				�Ϲ��ȴ�
	--9437401				���񡤻���
	--9437501				����ȴ�
	--9437601				��Ц�ȴ�

	--Tiaoxincishu = 0  --����������ҽ������ƴ���

	if act.GetActIsStart(153) then
		local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
		local s = map.IsInArea(3017299919, -1, RoleID, 6307)	--����true��false
		local t = map.IsInArea(3017299663, -1, RoleID, 10994)	--����true��false
		local Level = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local Bebuff_2 = role.IsRoleHaveBuff(MapID, InstanceID, RoleID,94373) -- ��¼����Ƿ��оϹ��ȴ�BUFF
		local Bebuff_6 = role.IsRoleHaveBuff(MapID, InstanceID, RoleID,94376) -- ��¼����Ƿ��д�Ц�ȴ�BUFF
		local Bebuff_7 = role.IsRoleHaveBuff(MapID, InstanceID, RoleID,94374) -- ��¼����Ƿ������񡤻���BUFF
		local Bebuff_10 = role.IsRoleHaveBuff(MapID, InstanceID, RoleID,94372) -- ��¼����Ƿ��д�Ц�ȴ�BUFF
		local Bebuff_13 = role.IsRoleHaveBuff(MapID, InstanceID, RoleID,94375) -- ��¼����Ƿ��п���ȴ�BUFF

		if MotionID== 6 then -- ��Ц���� ���ݵȼ���ӽ���
			if Guaiwuyuebing == 1 then -- �����ı���ʼ
				if s == true then  -- �Ƿ���������
					if Bebuff_6  == false then
						if Level >= 80 then
							role.AddRoleItem(MapID, InstanceID, RoleID, 6000113, 1, -1, 8, 420)	-- ��ӹ�������
						else
							role.AddRoleItem(MapID, InstanceID, RoleID, 3303031, 1, -1, 8, 420)	-- ���ʢ������
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 26, 4015) -- ������Ǵ�Ц��ʿ���õ����������ϵͳ����Ľ���
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
						end
						unit.AddBuff(MapID, InstanceID, RoleID, 9437601 ,RoleID)
					end
				end
			end
		end

		if MotionID == 10 then -- ���ƶ��� �ٻ��ӱ�
			if Guaiwuyuebing == 1 then
				if s == true then
					if Bebuff_10 == false then
						Tiaoxincishu = Tiaoxincishu +1  -- ������ʹ�����ƶ�������+1
						if Tiaoxincishu < 100 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 26, 4016) -- ������ƶ������ƺ���ĳֻ�����ŭ��
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
							if	Level >0 and Level <=15 then
								map.MapCreateColCreature(MapID, InstanceID, 1535410, x, y, z, 1)
							elseif
								Level >16 and Level <=30 then
								map.MapCreateColCreature(MapID, InstanceID, 1535411, x, y, z, 1)
							elseif
								Level >31 and Level <=45 then
								map.MapCreateColCreature(MapID, InstanceID, 1535412, x, y, z, 1)
							elseif
								Level >46 and Level <=60 then
								map.MapCreateColCreature(MapID, InstanceID, 1535413, x, y, z, 1)
							elseif
								Level >61 and Level <=75 then
								map.MapCreateColCreature(MapID, InstanceID, 1535414, x, y, z, 1)
							else
								map.MapCreateColCreature(MapID, InstanceID, 1535415, x, y, z, 1)
							end
						else
							if  Tiaoxincishu%100 == 0 then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 100, 4017)--����Ƶ��������Ϣ�������xx�Թ����ı������˵�xx�����ƣ���ŭ�Ĺ����������̲�ס�ˣ���Ҫ����λ�����߽�������������
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.AddMsgEvent(MsgID, 12, Tiaoxincishu)
								msg.DispatchWorldMsgEvent(MsgID)
								map.MapCreateColCreature(MapID, InstanceID, 1535410, 1683, 26847, 719, 1)
								map.MapCreateColCreature(MapID, InstanceID, 1535415, 1690, 26847, 723, 1)
								map.MapCreateColCreature(MapID, InstanceID, 1535411, 1700, 26847, 729, 1)
								map.MapCreateColCreature(MapID, InstanceID, 1535414, 1712, 26847, 735, 1)
								map.MapCreateColCreature(MapID, InstanceID, 1535412, 1723, 26847, 730, 1)
								map.MapCreateColCreature(MapID, InstanceID, 1535413, 1732, 26847, 727, 1)
								map.MapCreateColCreature(MapID, InstanceID, 1535413, 1741, 26847, 722, 1)
								map.MapCreateColCreature(MapID, InstanceID, 1535412, 1752, 26847, 714, 1)
								map.MapCreateColCreature(MapID, InstanceID, 1535414, 1754, 26847, 702, 1)
								map.MapCreateColCreature(MapID, InstanceID, 1535411, 1754, 26847, 696, 1)
								map.MapCreateColCreature(MapID, InstanceID, 1535415, 1755, 26847, 682, 1)
								map.MapCreateColCreature(MapID, InstanceID, 1535410, 1749, 26847, 668, 1)
							else
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 26, 4016) -- ������ƶ������ƺ���ĳֻ�����ŭ��
								msg.DispatchRoleMsgEvent(RoleID, MsgID)
								if	Level >0 and Level <=15 then
									map.MapCreateColCreature(MapID, InstanceID, 1535410, x, y, z, 1)
								elseif
									Level >16 and Level <=30 then
									map.MapCreateColCreature(MapID, InstanceID, 1535411, x, y, z, 1)
								elseif
									Level >31 and Level <=45 then
									map.MapCreateColCreature(MapID, InstanceID, 1535412, x, y, z, 1)
								elseif
									Level >46 and Level <=60 then
									map.MapCreateColCreature(MapID, InstanceID, 1535413, x, y, z, 1)
								elseif
									Level >61 and Level <=75 then
									map.MapCreateColCreature(MapID, InstanceID, 1535414, x, y, z, 1)
								else
									map.MapCreateColCreature(MapID, InstanceID, 1535415, x, y, z, 1)
								end
							end
						end
						unit.AddBuff(MapID, InstanceID, RoleID, 9437201 ,RoleID)
					end
				end
			end
		end

		if MotionID == 2 then -- �Ϲ�
			local exp = math.floor(activity_newexp[Level]*0.25)
			if Npcyuebing == 1 then -- NPC�ı���ʼ
				if t == true then
					if Bebuff_2 == false then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 4018) -- ��ʿ�ǾϹ���ʿ���õ����������ϵͳ����Ľ���
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						role.AddRoleExp(MapID, InstanceID, RoleID, exp)
						unit.AddBuff(MapID, InstanceID, RoleID, 9437301 ,RoleID)
					end
				end
			end
		end

		if MotionID == 7 then -- ����
			if Npcyuebing == 1 then -- NPC�ı���ʼ
				if t == true then
					if Bebuff_7 == false then
						if Level >= 30 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 26, 4019) -- ��ʿ������ʿ���õ����������ϵͳ����Ľ���
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
							unit.AddBuff(MapID, InstanceID, RoleID, 9437401 ,RoleID)
						end
					end
				end
			end
		end

		if MotionID == 13 then -- ����
			if Npcyuebing == 1 then -- NPC�ı���ʼ
				if t == true then
					if Bebuff_13 == false then
						if Level >= 60 then
							local MsgID = msg.BeginMsgEvent()
							msg.AddMsgEvent(MsgID, 26, 4020) -- ��ʿ�ǿ��㣬ʿ���õ����������ϵͳ����Ľ���
							msg.DispatchRoleMsgEvent(RoleID, MsgID)
							if role.GetSoarValue(RoleID) == 1 then -- ��ҽ�ɫ�ѷ���
								role.AddRoleItem(MapID, InstanceID, RoleID, 3300104, 2, -1, 8, 420)	-- �ɽ������
							end
							if role.GetSoarValue(RoleID) == 0 then -- ��ҽ�ɫδ����
								role.AddRoleItem(MapID, InstanceID, RoleID, 3300102, 1, -1, 8, 420) --�ɽ�ɽ���
								role.AddRoleItem(MapID, InstanceID, RoleID, 3300103, 1, -1, 8, 420) --�ɽ������
							end
							unit.AddBuff(MapID, InstanceID, RoleID, 9437501 ,RoleID)
						end
					end
				end
			end
		end
	end
--***************************************	 ����������������ʹ�ø��Զ���	******************************************************


end

-- ע����Ӧ�¼�
aux.RegisterMotionEvent(0, "Motion_OnUse")

