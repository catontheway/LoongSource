


--������Ի�
--RoleDataType["zhongqiu_yueliangshi"]     = 148  		--����������ʹ�ð�����ʯ��������

zhongqiu_kanfahenji = 0 	--��¼��ǰ�������У�����ѶԹ��������ĺۼ��������������ÿ��20:00����ֵ��ʼ��

function GuiShu_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

    --local curmon = tonumber(os.date("%m"))
	--local curday = tonumber(os.date("%d"))
	--local curhour = tonumber(os.date("%H")) -- ��ȡ��������ǰСʱ��
	--local curmin = os.date("%M")  -- ��ȡ��������ǰ������
	local	Num = role.GetRoleScriptData(RoleID, 1, RoleDataType["zhongqiu_yueliangshi"]) --��¼���ÿ�ջ������ʯ�ĸ���
	local  Level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local  Bebuff = role.IsRoleHaveBuff(MapID, InstanceID, RoleID,89999) -- ��¼��һ������ԭʯʱ�� 24Сʱ		8999980

	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 3907)	-- "��ѡ֮�������"
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)	-- ѡ��һ
		msg.AddMsgEvent(MsgID, 1, 3908)	-- "���ſ�һ�£���Ҫ��ɫ��ͷ����ȷ��������һ���ռ䣩"
		msg.AddMsgEvent(MsgID, 21, 5)	-- ѡ���
		msg.AddMsgEvent(MsgID, 1, 3909)	-- "�鿴�����ĺۼ�"
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

	if TalkIndex == 4 then  -- ���ſ�һ��
			-- ����ұ����еĽ��ӽ����ж�
		if role.GetRoleItemNum(RoleID, 6000006) == 0 then -- ��������н�ɫ��ͷ����Ϊ0
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 3910)	--��ʾ:"��ȱ�ٵ��߽�ɫ��ͷ���޷����п�����\n��ɫ��ͷ���Դ��϶���ȡ�����ߴ���մ�����
			msg.DispatchRoleMsgEvent(RoleID,MsgID)
		else
			zhongqiu_kanfahenji = zhongqiu_kanfahenji + 1 	-- ÿ����һ�ο����������������� +1
			local a = math.floor(zhongqiu_kanfahenji/1000)				-- 	��ȡ������ľ��ǧλ����
			local b = math.floor((zhongqiu_kanfahenji-a*1000)/100)		--	��ȡ������ľ�İ�λ����
			local c = math.floor((zhongqiu_kanfahenji-a*1000-b*100)/10)	--	��ȡ������ľ��ʮλ����
			local d = math.floor((zhongqiu_kanfahenji-a*1000-b*100-c*10))	--	��ȡ������ľ�ĸ�λ����
			if zhongqiu_kanfahenji >= 10000 then
				local n = math.random(1,100)
				if n == 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 3911)   --������ʾ��һ�õ���
					msg.AddMsgEvent(MsgID, 4, 6000001) --��õ�������
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 3911)--������Ϣ����ʾ��һ�õ���
					msg.AddMsgEvent(MsgID, 4, 6000001) --��õ�������
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					local MsgID = msg.BeginMsgEvent()   -- ���繫��
					msg.AddMsgEvent(MsgID, 100, 3922)--������Ϣ����ʾ��һ�õ���
					msg.AddMsgEvent(MsgID, 2, RoleID)
					msg.AddMsgEvent(MsgID, 4, 6000001) --��õ�������
					msg.DispatchWorldMsgEvent(MsgID)
				role.AddRoleItem(MapID, InstanceID, RoleID, 6000001, 1, -1, 8, 420) 	-- 1%���ʻ����ѡ֮�˳ƺŵ���
				end
				local k = math.random(50)
				local l = math.random(600)
				local exp = Level*50
				if k == 1 and l ~= 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 3918)   --�� ��С�Ĵ�ŭ���������⵽�췣�ˡ���
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					unit.AddBuff(MapID, InstanceID, RoleID, 5005501 ,RoleID)
				elseif
					l == 1 and k ~= 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 3911)   --������ʾ��һ�õ���
					msg.AddMsgEvent(MsgID, 4, 3304163)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					role.AddRoleItem(MapID, InstanceID, RoleID, 3304163, 1, -1, 8, 420) -- ��ӵͼ��ػ�ʯ  100��
					local MsgID = msg.BeginMsgEvent()   -- ���繫��
					msg.AddMsgEvent(MsgID, 100, 3922)--������Ϣ����ʾ��һ�õ���
					msg.AddMsgEvent(MsgID, 2, RoleID)
					msg.AddMsgEvent(MsgID, 4, 3304163) --��õ�������
					msg.DispatchWorldMsgEvent(MsgID)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 3914)   --����������һЩ���飬�ƺ�û��ʲô������Ч���������ٿ�һ�£�Ҳ����о�ϲ
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					role.AddRoleExp(MapID, InstanceID, RoleID, exp)
				end

			else
				--	���������β������ʱ����õ��߼���Ǯ
				if  a == 9 and b == 9 and c ==9 and d == 9  then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 3912)   --������ʾ��һ�õ��߼���Ǯ
					msg.AddMsgEvent(MsgID, 12, 9999) -- ���������Ĵ���
					msg.AddMsgEvent(MsgID, 4, 6000002) --��õ�������
					msg.AddMsgEvent(MsgID, 12, 50000) -- ��ý�Ǯ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 3912)--������Ϣ����ʾ��һ�õ��߼���Ǯ
					msg.AddMsgEvent(MsgID, 12, 9999) -- ���������Ĵ���
					msg.AddMsgEvent(MsgID, 4, 6000002) --��õ�������
					msg.AddMsgEvent(MsgID, 12, 50000) -- ��ý�Ǯ����
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					local MsgID = msg.BeginMsgEvent()   -- ���繫��
					msg.AddMsgEvent(MsgID, 100, 3919)--������Ϣ����ʾ��һ�õ���
					msg.AddMsgEvent(MsgID, 2, RoleID)
					msg.AddMsgEvent(MsgID, 12, 9999)
					msg.AddMsgEvent(MsgID, 4, 6000002) --��õ�������
					msg.AddMsgEvent(MsgID, 12, 50000) -- ��ý�Ǯ����
					msg.DispatchWorldMsgEvent(MsgID)
					role.AddRoleSilver(MapID, InstanceID, RoleID, 500000000, 102)	-- ���50000����
					role.AddRoleItem(MapID, InstanceID, RoleID, 6000002, 1, -1, 8, 420) -- �����ѡ֮�˳ƺ�(3days)
				elseif
					a == 6 and b == 6 and c == 6 and d == 6 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 3912)
					msg.AddMsgEvent(MsgID, 12, 6666)
					msg.AddMsgEvent(MsgID, 4, 6000001)
					msg.AddMsgEvent(MsgID, 12, 50000)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 3912)
					msg.AddMsgEvent(MsgID, 12, 6666)
					msg.AddMsgEvent(MsgID, 4, 6000001)
					msg.AddMsgEvent(MsgID, 12, 50000)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 100, 3919)--������Ϣ����ʾ��һ�õ���
					msg.AddMsgEvent(MsgID, 2, RoleID)
					msg.AddMsgEvent(MsgID, 12, 6666)
					msg.AddMsgEvent(MsgID, 4, 6000001) --��õ�������
					msg.AddMsgEvent(MsgID, 12, 50000) -- ��ý�Ǯ����
					msg.DispatchWorldMsgEvent(MsgID)
					role.AddRoleSilver(MapID, InstanceID, RoleID, 500000000, 102)	-- ���50000����
					role.AddRoleItem(MapID, InstanceID, RoleID, 6000001, 1, -1, 8, 420) -- �����ѡ֮�˳ƺ�(1days)
				elseif
					a == 3 and b == 6 and c == 6 and d == 6 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 3913)
					msg.AddMsgEvent(MsgID, 12, 3666)
					msg.AddMsgEvent(MsgID, 12, 15000)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 3913)
					msg.AddMsgEvent(MsgID, 12, 3666)
					msg.AddMsgEvent(MsgID, 12, 15000)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 100, 3920)--������Ϣ����ʾ��һ�õ���
					msg.AddMsgEvent(MsgID, 2, RoleID)
					msg.AddMsgEvent(MsgID, 12, 3666)
					msg.AddMsgEvent(MsgID, 12, 15000) -- ��ý�Ǯ����
					msg.DispatchWorldMsgEvent(MsgID)
					role.AddRoleSilver(MapID, InstanceID, RoleID, 150000000, 102)	-- ���15000����
				elseif
					a == 0 and b == 6 and c == 6 and d == 6 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 3913)
					msg.AddMsgEvent(MsgID, 12, 666)
					msg.AddMsgEvent(MsgID, 12, 5000)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 3913)
					msg.AddMsgEvent(MsgID, 12, 666)
					msg.AddMsgEvent(MsgID, 12, 5000)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 100, 3920)--������Ϣ����ʾ��һ�õ���
					msg.AddMsgEvent(MsgID, 2, RoleID)
					msg.AddMsgEvent(MsgID, 12, 666)
					msg.AddMsgEvent(MsgID, 12, 5000) -- ��ý�Ǯ����
					msg.DispatchWorldMsgEvent(MsgID)
					role.AddRoleSilver(MapID, InstanceID, RoleID, 50000000, 102)	-- ���5000����
				elseif
					a == 0 and b == 0 and c == 6 and d == 6 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 3913)
					msg.AddMsgEvent(MsgID, 12, 66)
					msg.AddMsgEvent(MsgID, 12, 500)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 3913)
					msg.AddMsgEvent(MsgID, 12, 66)
					msg.AddMsgEvent(MsgID, 12, 500)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 100, 3920)--������Ϣ����ʾ��һ�õ���
					msg.AddMsgEvent(MsgID, 2, RoleID)
					msg.AddMsgEvent(MsgID, 12, 66)
					msg.AddMsgEvent(MsgID, 12, 500) -- ��ý�Ǯ����
					msg.DispatchWorldMsgEvent(MsgID)
					role.AddRoleSilver(MapID, InstanceID, RoleID, 5000000, 102)	-- ���500����
				else
					local k = math.random(50)
					local l = math.random(600)
					local exp = Level*50
					if k == 1 and l ~= 1 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 3918)   --�� ��С�Ĵ�ŭ���������⵽�췣�ˡ���
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						unit.AddBuff(MapID, InstanceID, RoleID, 5005501 ,RoleID)
					elseif
						l == 1 and k ~= 1 then
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 3911)   --������ʾ��һ�õ���
						msg.AddMsgEvent(MsgID, 4, 3304163)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()   -- ���繫��
						msg.AddMsgEvent(MsgID, 100, 3922)--������Ϣ����ʾ��һ�õ���
						msg.AddMsgEvent(MsgID, 2, RoleID)
						msg.AddMsgEvent(MsgID, 4, 3304163) --��õ�������
						msg.DispatchWorldMsgEvent(MsgID)
						role.AddRoleItem(MapID, InstanceID, RoleID, 3304163, 1, -1, 8, 420) -- ��ӵͼ��ػ�ʯ  100��
					else
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 3914)   --����������һЩ���飬�ƺ�û��ʲô������Ч���������ٿ�һ�£�Ҳ����о�ϲ
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						role.AddRoleExp(MapID, InstanceID, RoleID, exp)
					end
				end
			end
		--ÿ������β��Ϊ6ʱ����õ���
			if  d == 6 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 3915)
				msg.AddMsgEvent(MsgID, 4, 6000003)
				msg.AddMsgEvent(MsgID, 4, 6000004)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 0)
				msg.AddMsgEvent(MsgID, 1, 3915)
				msg.AddMsgEvent(MsgID, 4, 6000003)
				msg.AddMsgEvent(MsgID, 4, 6000004)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				role.AddRoleItem(MapID, InstanceID, RoleID, 6000003, 1, -1, 8, 420) -- ��ӹ�����֦
				role.AddRoleItem(MapID, InstanceID, RoleID, 6000004, 1, -1, 8, 420) -- ����¹ⱦ��
			end

		--ÿ��������ʯ���� ������10��

				if Num <10  and Bebuff ~= true   then
				    local k = math.random(150)	-- 1.5%���ʻ���¹�ԭʯ
				    if 	k == 1 then
					Num = Num +1
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 3916)
					msg.AddMsgEvent(MsgID, 12, Num)
					msg.AddMsgEvent(MsgID, 4, 6000010)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 3916)
					msg.AddMsgEvent(MsgID, 12, Num)
					msg.AddMsgEvent(MsgID, 4, 6000010)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					role.AddRoleItem(MapID, InstanceID, RoleID, 6000010, 1, -1, 8, 420) -- 1.5%���ʻ���¹�ԭʯ
					role.SetRoleScriptData(RoleID, 1, RoleDataType["zhongqiu_yueliangshi"], Num) --��ɫ����¹�ԭʯ���� +1
				    end
				end
			if  Num == 10 then   	-- ÿ�����ϻ���¹�ԭʯ�������ܳ���10��
				unit.AddBuff(MapID, InstanceID, RoleID, 8999980 ,RoleID) --��������24Сʱ��BUFF�����ڼ�¼����ԭʯ���ʱ��
				role.SetRoleScriptData(RoleID, 1, RoleDataType["zhongqiu_yueliangshi"], 0)
			end
		role.RemoveFromRole(MapID, InstanceID, RoleID, 6000006, 1, 420)
		end
	end

	if TalkIndex == 5 then  -- �鿴�����ĺۼ�
		local a =  math.floor(zhongqiu_kanfahenji/1000)					-- 	��ȡ������ľ��ǧλ����
		local b =  math.floor((zhongqiu_kanfahenji-a*1000)/100)			--	��ȡ������ľ�İ�λ����
		local c =  math.floor((zhongqiu_kanfahenji-a*1000-b*100)/10)		--	��ȡ������ľ��ʮλ����
		local d =  math.floor((zhongqiu_kanfahenji-a*1000-b*100-c*10))	--	��ȡ������ľ�ĸ�λ����
		if	a ~= 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 3917)	--��ʾ:"����������ԼԼ��Щ�ۼ����ִ�����һ�£������xxxx�����ϡ���--����������Ϊ��λ��ʱ����ʾ��ȷ����
			msg.AddMsgEvent(MsgID, 12, a*1000)
			msg.DispatchRoleMsgEvent(RoleID,MsgID)
		elseif a == 0 and b ~= 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 3917)	--��ʾ:"����������ԼԼ��Щ�ۼ����ִ�����һ�£������xxxx�����ϡ���--����������Ϊ��λ��ʱ����ʾ��ȷ����
			msg.AddMsgEvent(MsgID, 12, b*100)
			msg.DispatchRoleMsgEvent(RoleID,MsgID)
		elseif a == 0 and b == 0 and c ~= 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 3917)	--��ʾ:"����������ԼԼ��Щ�ۼ����ִ�����һ�£������xxxx�����ϡ���--����������Ϊ��λ��ʱ����ʾ��ȷ����
			msg.AddMsgEvent(MsgID, 12, c*10)
			msg.DispatchRoleMsgEvent(RoleID,MsgID)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 3917)	--��ʾ:"����������ԼԼ��Щ�ۼ����ִ�����һ�£������xxxx�����ϡ���--����������Ϊ��λ��ʱ����ʾ��ȷ����
			msg.AddMsgEvent(MsgID, 12, zhongqiu_kanfahenji)
			msg.DispatchRoleMsgEvent(RoleID,MsgID)
		end
	end
end

aux.RegisterCreatureEvent(3610221, 7, "GuiShu_OnTalk")










