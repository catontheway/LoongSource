

YuanXiaoGouHuo = {}          --��Ÿõ�ͼ�����𱻵�ȼ�Ĵ���
YuanXiaoGouHuo_Burning = {}  --��¼�����ȼ��״̬
YuanXiao_CreatureID = {}			-- ��Ŵ���Ԫ�����ص�id�����ڻ������ɾ��npc
--4900427	Ԫ������
--9428501	��������


function YuanXiao_OnStart(actID)
	-- �㲥Ԫ�����ʼ
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 100, 2118)    --���̻����һ��ƣ����ֵ�Ԫ�����ʼ�ˣ���ľ�����������ڸ������ǵĸ����´�˵���˴���ѯ��
	msg.DispatchWorldMsgEvent(MsgID)
	YuanXiao_CreatureID[1] = map.MapCreateCreature(3017298127, -1, 4900422, 2208, 6646, 2235)

end

function YuanXiao_OnTimerMin(actID)
    if yuanxiaogouhuo_Time() then
		YuanXiaoGouHuo = {}          --��Ÿõ�ͼ�����𱻵�ȼ�Ĵ���
		YuanXiaoGouHuo[3017298127] = 0
		YuanXiaoGouHuo[3017298383] = 0
		YuanXiaoGouHuo[3017299663] = 0
		YuanXiaoGouHuo[3017299919] = 0
		YuanXiaoGouHuo[3017299151] = 0
		YuanXiaoGouHuo[3017299407] = 0
		YuanXiaoGouHuo_Burning = {}  --�ÿ������ȼ��״̬
		YuanXiaoGouHuo_Burning[3017298127] = {}
		YuanXiaoGouHuo_Burning[3017298383] = {}
		YuanXiaoGouHuo_Burning[3017299663] = {}
		YuanXiaoGouHuo_Burning[3017299919] = {}
		YuanXiaoGouHuo_Burning[3017299151] = {}
		YuanXiaoGouHuo_Burning[3017299407] = {}
		--��������ʼ��������ܿ��Ҫ�����ˣ�����ٶ�Ѱ�Ҹ����е�����ѣ����칬���ֽ����ȼ��������ȼ����ѻ��о�ϲŶ��
		local MsgID = msg.BeginMsgEvent()
	    msg.AddMsgEvent(MsgID, 100, 2119)
	    msg.DispatchWorldMsgEvent(MsgID)

	    --map.MapCreateColCreature(3017298127, -1, 1534112, 2212, 6646, 2233, 1)
	    map.MapCreateCreature(3017298127, -1, 4900427, 2212, 6646, 2241)
		map.MapCreateCreature(3017298127, -1, 4900427, 2204, 6646, 2234)
		map.MapCreateCreature(3017298127, -1, 4900427, 2210, 6646, 2225)
		map.MapCreateCreature(3017298127, -1, 4900427, 2217, 6646, 2228)
		map.MapCreateCreature(3017298127, -1, 4900427, 2219, 6646, 2235)

        --map.MapCreateColCreature(3017298383, -1, 1534112, 822, 10000, 1994, 1)
		map.MapCreateCreature(3017298383, -1, 4900427, 827, 10000, 1985)
		map.MapCreateCreature(3017298383, -1, 4900427, 837, 10000, 1984)
		map.MapCreateCreature(3017298383, -1, 4900427, 841, 10000, 1994)
		map.MapCreateCreature(3017298383, -1, 4900427, 831, 10000, 2003)
		map.MapCreateCreature(3017298383, -1, 4900427, 822, 10000, 1994)

		--map.MapCreateColCreature(3017299663, -1, 1534112, 2094, 18345, 2678, 1)
		map.MapCreateCreature(3017299663, -1, 4900427, 2098, 18345, 2688)
		map.MapCreateCreature(3017299663, -1, 4900427, 2104, 18345, 2676)
		map.MapCreateCreature(3017299663, -1, 4900427, 2097, 18345, 2667)
		map.MapCreateCreature(3017299663, -1, 4900427, 2084, 18345, 2673)
		map.MapCreateCreature(3017299663, -1, 4900427, 2084, 18345, 2687)

		--map.MapCreateColCreature(3017299919, -1, 1534112, 890, 20140, 1004, 1)
		map.MapCreateCreature(3017299919, -1, 4900427, 898, 20140, 994)
		map.MapCreateCreature(3017299919, -1, 4900427, 910, 20140, 998)
		map.MapCreateCreature(3017299919, -1, 4900427, 910, 20140, 1010)
		map.MapCreateCreature(3017299919, -1, 4900427, 898, 20140, 1013)
		map.MapCreateCreature(3017299919, -1, 4900427, 890, 20140, 1004)

		--map.MapCreateColCreature(3017299151, -1, 1534112, 2069, 6365, 1783, 1)
		map.MapCreateCreature(3017299151, -1, 4900427, 2077, 6365, 1771)
		map.MapCreateCreature(3017299151, -1, 4900427, 2069, 6365, 1760)
		map.MapCreateCreature(3017299151, -1, 4900427, 2057, 6365, 1778)
		map.MapCreateCreature(3017299151, -1, 4900427, 2057, 6365, 1764)
		map.MapCreateCreature(3017299151, -1, 4900427, 2069, 6365, 1783)

		--map.MapCreateColCreature(3017299407, -1, 1534112, 1881, 14024, 2296, 1)
		map.MapCreateCreature(3017299407, -1, 4900427, 1847, 14232, 1921)
		map.MapCreateCreature(3017299407, -1, 4900427, 1859, 14204, 1921)
		map.MapCreateCreature(3017299407, -1, 4900427, 1862, 14175, 1930)
		map.MapCreateCreature(3017299407, -1, 4900427, 1854, 14174, 1937)
		map.MapCreateCreature(3017299407, -1, 4900427, 1845, 14209, 1931)

		map.MapCreateColCreature(3017298127, -1, 1534113, 3097, 6958, 2551, 1)
		map.MapCreateColCreature(3017298127, -1, 1534113, 3393, 4204, 969, 1)
		map.MapCreateColCreature(3017298127, -1, 1534113, 1383, 10581, 3607, 1)
        map.MapCreateColCreature(3017298383, -1, 1534114, 727, 8618, 1318, 1)
		map.MapCreateColCreature(3017298383, -1, 1534114, 1733, 10764, 2187, 1)
		map.MapCreateColCreature(3017298383, -1, 1534114, 2262, 9963, 1659, 1)
		map.MapCreateColCreature(3017299663, -1, 1534115, 1795, 12634, 1573, 1)
		map.MapCreateColCreature(3017299663, -1, 1534115, 2739, 19793, 3109, 1)
		map.MapCreateColCreature(3017299663, -1, 1534115, 2352, 12668, 929, 1)
		map.MapCreateColCreature(3017299919, -1, 1534116, 1310, 23165, 2040, 1)
		map.MapCreateColCreature(3017299919, -1, 1534116, 2386, 21028, 1756, 1)
		map.MapCreateColCreature(3017299919, -1, 1534116, 2016, 16733, 2072, 1)
		map.MapCreateColCreature(3017299151, -1, 1534117, 1361, 5871, 1353, 1)
		map.MapCreateColCreature(3017299151, -1, 1534117, 1995, 3180, 929, 1)
		map.MapCreateColCreature(3017299151, -1, 1534117, 2583, 4695, 1814, 1)
		map.MapCreateColCreature(3017299407, -1, 1534118, 1174, 9187, 3071, 1)
		map.MapCreateColCreature(3017299407, -1, 1534118, 1302, 10880, 476, 1)
		map.MapCreateColCreature(3017299407, -1, 1534118, 2529, 8163, 1677, 1)
	elseif yuanxiaoguaiwu_Time() then
		map.MapCreateColCreature(3017298127, -1, 1534113, 3097, 6958, 2551, 1)
		map.MapCreateColCreature(3017298127, -1, 1534113, 3393, 4204, 969, 1)
		map.MapCreateColCreature(3017298127, -1, 1534113, 1383, 10581, 3607, 1)
        map.MapCreateColCreature(3017298383, -1, 1534114, 727, 8618, 1318, 1)
		map.MapCreateColCreature(3017298383, -1, 1534114, 1733, 10764, 2187, 1)
		map.MapCreateColCreature(3017298383, -1, 1534114, 2262, 9963, 1659, 1)
		map.MapCreateColCreature(3017299663, -1, 1534115, 1795, 12634, 1573, 1)
		map.MapCreateColCreature(3017299663, -1, 1534115, 2739, 19793, 3109, 1)
		map.MapCreateColCreature(3017299663, -1, 1534115, 2352, 12668, 929, 1)
		map.MapCreateColCreature(3017299919, -1, 1534116, 1310, 23165, 2040, 1)
		map.MapCreateColCreature(3017299919, -1, 1534116, 2386, 21028, 1756, 1)
		map.MapCreateColCreature(3017299919, -1, 1534116, 2016, 16733, 2072, 1)
		map.MapCreateColCreature(3017299151, -1, 1534117, 1361, 5871, 1353, 1)
		map.MapCreateColCreature(3017299151, -1, 1534117, 1995, 3180, 929, 1)
		map.MapCreateColCreature(3017299151, -1, 1534117, 2583, 4695, 1814, 1)
		map.MapCreateColCreature(3017299407, -1, 1534118, 1174, 9187, 3071, 1)
		map.MapCreateColCreature(3017299407, -1, 1534118, 1302, 10880, 476, 1)
		map.MapCreateColCreature(3017299407, -1, 1534118, 2529, 8163, 1677, 1)
	end
	if yuanxiaogonggao_Time() then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 2120)    --Ԫ���ڻ��ʼ���������ʹ�Ѿ������ˣ�����ٶ�Ѱ�Ҹ������еĻ������ʹ���������ϴ����칬���֣����кܶ���õı�����
		msg.DispatchWorldMsgEvent(MsgID)
	end
end

-- npc����ʱ�䵽����������ڸ�������ɾ����npc
function YuanXiao_OnEnd(actID)
	map.MapDeleteCreature(3017298127, -1, YuanXiao_CreatureID[1])

end

--ע��û������¼�
aux.RegisterActEvent(76, 2, "YuanXiao_OnStart")
aux.RegisterActEvent(76, 3, "YuanXiao_OnEnd")
aux.RegisterActEvent(76, 4, "YuanXiao_OnTimerMin")


--����Ԫ������
function C4900427_OnInVest(MapID, InstanceID, CreatureID, CreatureTypeID, RoleID)
    --local bhave = role.IsRoleHaveBuff(MapID, InstanceID, CreatureID, 94285)
	if YuanXiaoGouHuo_Burning[MapID][CreatureID] == 1 then
	    --��ʾ�����������Ѿ���ȼ��������һ������������
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 2121)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	else
	    if 0 == role.RemoveFromRole(MapID, InstanceID, RoleID, 3303129, 1, 1000) then
			YuanXiaoGouHuo[MapID] = YuanXiaoGouHuo[MapID] + 1
			role.SigTitleEvent(RoleID, 188)--��óƺ� �����ܸ�
			if YuanXiaoGouHuo[MapID] ~= 50 and YuanXiaoGouHuo[MapID] % 10 ==0 then
				unit.AddBuff(MapID, InstanceID, CreatureID, 9428501, CreatureID)
				YuanXiaoGouHuo_Burning[MapID][CreatureID] = 1
				local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
				level = math.floor(level/10)
				role.AddRoleExp(MapID, InstanceID, RoleID, Yuan_Dan_Exp[level])
			elseif YuanXiaoGouHuo[MapID] % 10 ~=0 and YuanXiaoGouHuo[MapID] ~= nil then
				local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
				level = math.floor(level/10)
				role.AddRoleExp(MapID, InstanceID, RoleID, Yuan_Dan_Exp[level])
			elseif YuanXiaoGouHuo[MapID] == 50 then
			    unit.AddBuff(MapID, InstanceID, CreatureID, 9428501, CreatureID)
				YuanXiaoGouHuo_Burning[MapID][CreatureID] = 1
				local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
				level = math.floor(level/10)
				role.AddRoleExp(MapID, InstanceID, RoleID, Yuan_Dan_Exp[level])
			    if MapID == 3017298127 then
				    map.MapCreateColCreature(3017298127, -1, 1534112, 2212, 6646, 2233, 1)
                elseif MapID == 3017298383 then
				    map.MapCreateColCreature(3017298383, -1, 1534112, 832, 10000, 1992, 1)
				elseif MapID == 3017299663 then
                    map.MapCreateColCreature(3017299663, -1, 1534112, 2094, 18345, 2678, 1)
				elseif MapID == 3017299919 then
				    map.MapCreateColCreature(3017299919, -1, 1534112, 901, 20140, 1004, 1)
				elseif MapID == 3017299151 then
				    map.MapCreateColCreature(3017299151, -1, 1534112, 2067, 6365, 1771, 1)
				elseif MapID == 3017299407 then
				    map.MapCreateColCreature(3017299407, -1, 1534112, 1853, 14198, 1928, 1)
				end
				local MsgID = msg.BeginMsgEvent()
			    msg.AddMsgEvent(MsgID, 13, 1)
			    msg.AddMsgEvent(MsgID, 1, 2122) --��������٣���ҿ�콫����ܣ������Ͽɴ��źö�ñ�������
			    msg.DispatchMapMsgEvent(MsgID, MapID, -1)
			end
		else
		    --��ʾ����Ԫ��������Ҫʹ�û�������Ľ����칬������ܵ�ȼ��
		    local MsgID = msg.BeginMsgEvent()
		    msg.AddMsgEvent(MsgID, 26, 2123)
		    msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end

--ע�����Ԫ�������¼�
aux.RegisterCreatureEvent(4900427, 6, "C4900427_OnInVest")


